# nix

This notes assume to have the [`experimental-features = nix-command flakes`][nix-conf-cmds] enabled.

## nix.conf ([ref][nix-conf])
```bash
nix config show
# experimental-features
#     enable additional experimental features
# flake-registery
#     url of global registry
# tarball-ttl
#     time the downloaded tarball is treated as valid
```

## nix search ([ref][nix-search])
```bash
nix search <installable> <regex>
nix search nixpkgs glibc
```

## nix profile ([ref][nix-profile])
The history allows to install packages, update them and easily rollback.
```bash
# Install/remove/upgrade package(s) (creates a new history version).
nix profile install <installable>
nix profile remove
nix profile upgrade --all

# List installed packages.
nix profile list

# Show different hisitory versions.
nix profile history

# Rollback profile history.
nix profile rollback [--to N]
```

## nix store ([ref][nix-store])
```bash
# Show all alive store paths (paths with active reference).
nix-store --gc --print-live
# Show all dead store paths (paths w/o active reference).
nix-store --gc --print-dead

# Run garbage collection on nix store. This deletes all paths which are no
# gcroots and any dependency of any gcroot.
nix store gc

# Delete single path from store.
nix store delete <path>

# gcroots are for example created automatically when installing packages into
# the profile or when building a flake locally.
# gcroots are located under /nix/var/nix/gcroots
#
# One can also create mamual gcroots (eg for tmp operation).
> nix-store --gc --print-dead | grep 'binutils-2.44$'
/nix/store/wcv8n2k53w8sbffpf716gxj6mjb5jf26-binutils-2.44

> mkdir -p /nix/var/nix/gcroots/tmp
> ln -s /nix/store/wcv8n2k53w8sbffpf716gxj6mjb5jf26-binutils-2.44 /nix/var/nix/gcroots/tmp/keep-binutils

> nix-store --gc --print-dead | grep 'binutils-2.44$'
> nix-store --gc --print-live | grep 'binutils-2.44$'
/nix/store/wcv8n2k53w8sbffpf716gxj6mjb5jf26-binutils-2.44
```

## nix flake ([ref][nix-flake])
Flakes provide a description for reproducible builds.

```bash
# Create a new flake in a folder with name.
nix flake new <name>
# Create a new flake from given template.
nix flake new -t <template> <name>
# Create a new flake with the c-hello output from the templates flake.
nix flake create -t templates#c-hello myflake

# Show the outputs of a flake
nix flake show <flake>
# Show outputs from the default "templates" flake.
nix flake show templates
# Assumes '.' as flake (which effectively means flake.nix in cwd).
nix flake show

# Show the derivation of an installable in pretty format.
nix derivation show <installable>
# Show the derivation of the glibc output from the nixpkg flake.
nix derivation show nixpkgs#glibc
# Assumes '.' as flake.
nix derivation show

# Show a flakes metadata (eg store path, inputs, description, ..)
nix flake metadata <flake>

# Update the versions in the flake.lock file.
nix flake update
```
> Use `nix registry list` to list available global flakes.
>
> Documentation for [installable][nix-installable] syntax.

### a bare bones flake
The following shows a *bare bones* flake using the
[`builtins.derivation`][builtin-drv] function to define an output called
**moose**. The builtin provides an empty environment.

In general, a derivation is a declarative description of how to run an
*executable (builder)* on a set of *inputs (input derivations aka dependencies)*
to produce a set of *outputs (store paths)*.
```nix
{{#include nix/bare-bones-derivation/flake.nix}}
```
Understanding this example is crucial as any abstractions such as the
[`nixpkgs.stdenv`][nixpkgs-stdenv] builds on top of this.

One can inspect and build the derivation as follows.
```bash
# Show the derivation for the output "moose".
> nix derivation show .#moose
{
  "/nix/store/q2l7g7myy5gmks7jml6hz2jd73yybplq-bone.drv": {
    ...
    "inputDrvs": {
      "/nix/store/d2gv7y7i7bw79dpfskzzk2g4h6mc0677-bash-interactive-5.2p37.drv": {
}

# Build the output 'moose' for the flake.nix in the cwd ('.').
> nix build .#moose ; cat result
hello
```
> `.#moose` references the flake.nix in the cwd `'.'` and the output `moose`.

An interesting point to observe is that nix computed *bash* as input derivation
(inputDrv) and hence as input dependency for this output.

### a stdenv flake

The [`stdenv`][nixpkgs-stdenv] library from the *nixpkgs* flake provides the
`mkDerivation` function to simplify building packages (defining derivations).
Many tools commonly used to build software are available out of the box.
Specifying build and runtime dependencies can also be done easiy.

The default builder provided by the `mkDerivation` function, organizes the build
into multiple phases like *config*, *build* and *install*. When defining the
derivation these phases can be easily overwritten.

The example show a simple build and installation of a C file.
```nix
{{#include nix/stdenv/flake.nix}}
```

One can also define multiple *output* directories using the `outputs` attribute
in the derivation. Each output turns into an environment variable of the same
name. Use `nix derivation show` to inspect the outputs and the environment the
builder inherits.

The for example the *glibc* package in the *nixpkgs* flake, which provides
multiple outputs.

```bash
> nix derivation show nixpkgs#glibc
..
"outputs": {
      ..
      "out": {
        "path": "/nix/store/g3s0z9r7m1lsfxdk8bj88nw8k8q3dmmg-glibc-2.40-66"
      },
      "static": {
        "path": "/nix/store/lfj0w1r0ppj2swb11pz8vkppmsy1rf6y-glibc-2.40-66-static"
      }stdenv/flake.nix
    },
..
```

### a development shell flake
The `nixpkgs` flake also provides the [`mkShell`][nixpkgs-mkshell] derivation
function (specialization of `mkDerivation`), which can be used to easily define
a development environment.


The examples shows an environment with the `zig` compiler and `zls`, the zig lsp
server. Running `nix develop` will drop in a shell where these packages are
available.
```nix
{{#include nix/devshell/flake.nix}}
```

## nix lang basics
Nix is a functional language, where everything is an expression.
The following shows enough nix lang to come quite far.

```nix
$ nix repl

nix-repl> 1 + 2
3

# String interpolation.
# (Assignment operation special to repl).
nix-repl> foo = "bar"
nix-repl> "hello ${foo}"
"hello bar"

# Simple function with argument.
nix-repl> f = a: a + 2
nix-repl> f 4
6

# Attribute sets (kv).
nix-repl> { a = 1; b = 2; }
{
  a = 1;
  b = 2;
}

# Function taking an attribute set.
nix-repl> f = { a, b }: a + b
nix-repl> f { a = 1; b = 2; }
3

# Defining local variables with "let .. in".
nix-repl> let x = 1; in x + 3
4
nix-repl> x
error: undefined variable 'x'

# Let .. in expression returning an attribute set (handy when defining derivations).
nix-repl> let x = "abc"; in { name = "bar ${x}"; }
{ name = "bar abc"; }

# Show all builtin functions.
nix-repl> builtins
{
  abort = «primop abort»;
  add = «primop add»;
  ..
```

## References

- [nix installable][nix-installable]
- [nix flake][nix-flake]
- [nix store][nix-store]
- [nix profile][nix-profile]
- [nix conf][nix-conf]
- [`builtin.derivation`][builtin-drv]
- [`nixpkgs.stdenv`][nixpkgs-stdenv]
- [`nixpkgs.mkShell`][nixpkgs-mkshell]
- [nix pills][nix-pills]


[nix-conf]: https://nix.dev/manual/nix/latest/command-ref/conf-file.html
[nix-conf-cmds]: https://nix.dev/manual/nix/latest/command-ref/conf-file.html#conf-experimental-features
[nix-search]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-search
[nix-profile]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-profile
[nix-flake]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-flake
[nix-store]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-store
[nix-installable]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix#installables

[builtin-drv]: https://nix.dev/manual/nix/latest/language/derivations
[nixpkgs-stdenv]: https://nixos.org/manual/nixpkgs/stable/#chap-stdenv
[nixpkgs-mkshell]: https://nixos.org/manual/nixpkgs/stable/#sec-pkgs-mkShell

[nix-pills]: https://nixos.org/guides/nix-pills/
