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
# Flakeref from registry.
nix search nixpkgs glibc
# Explicit flakeref.
nix search nixpkgs/nixos-25.05 glibc
```

## nix run ([ref][nix-run])
```bash
nix run <installable> -- <args>
nix run nixpkgs#strace -- -o strace.txt /bin/ls
nix run nixpkgs/nixos-25.05#emacs
```

## nix shell ([ref][nix-shell])
```bash
# Enter interactive shell with pkgs in PATH.
nix shell <installable> [<installable>]
nix shell nixpkgs#qemu nixpkgs#zig nixpkgs/nixos-25.05#strace
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
> Use `nix registry list` to list available global flakes or `nix registry pin
> <flake>`, to pin a flake to a fixed version.
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
> A derivation also defines a set of `outputs`, which by default is `["out"]`.
> Outputs are passed as environment variables to the builder, from where they
> can be used as install targets for example.

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

Alternatively there is `stdenvNoCC.mkDerivation` which provides an environment
without a C compiler.

The example show a simple build and installation of a C file.
```nix
{{#include nix/stdenv/flake.nix}}
```
> Use [`NIX_DEBUG=[0-7]`][nixpkgs-nixdebug] to enable `stdenv` debug logging.
> Use `nix log [<installable>]` to inspect the build log.

One can also define multiple *output* directories using the `outputs` attribute
in the derivation. Each output turns into an environment variable of the same
name. Use `nix derivation show` to inspect the outputs and the environment the
builder inherits.

For example the *glibc* package in the *nixpkgs* flake provides multiple
outputs.

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
      }
    },
..
```

### a development shell flake
The `nixpkgs` flake also provides the [`mkShell`][nixpkgs-mkshell] derivation
function (specialization of `mkDerivation`), which can be used to easily define
a development environment.

Alternatively there is also [`mkShellNoCC`][nixpkgs-mkshell-nocc] which provides
an environment without a C compiler.

The example shows an environment with the `zig` compiler and `zls`, the zig lsp
server. Running `nix develop` will drop into a shell where these packages are
available.
```nix
{{#include nix/devshell/flake.nix}}
```

When launching a development shell, the environment of the current shell is
inherited. Using `-i` clears the environment when entering the dev shell, using
`-k` certain variables can be kept.
For example the following command will only inherit `$USER, $HOME, $DISPLAY`
into the development shell.
```sh
nix develop -i -k USER -k HOME -k DISPLAY
```

One can also just run a command in the development shell.
```sh
nix develop --command bash -c "mkdir build && cd build && cmake .. && make"
```

### flake default targets
When called without an explicit output, certain nix commands have default
outputs. The following gives the most relevant.
```
nix build
nix run
nix log
# uses -> packages.<SYSTEM>.default

nix develop
# uses -> devShells.<SYSTEM>.default
```

### a flake for multiple systems
The `nixpkgs` flake also provides lots of useful library functions. One example
is the [`nixpkgs.lib.genAttrs`][nixpkgs-genattrs] function, which allows to
create an attribute set from a list of keys and a lambda function which computes
the values of the attribute set.

```nix
nix-repl> pkgs.lib.genAttrs [ "a" "b" ] (arg: "some-value-${arg}")
{
  a = "some-value-a";
  b = "some-value-b";
}
```

This can be used to build a flake to support multiple systems. The following
shows a small example for two systems which defines a dev shell and a formatter.
```nix
{{#include nix/for-systems/flake.nix}}
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

# Attribute sets (kv).
nix-repl> { a = 1; b = 2; }
{
  a = 1;
  b = 2;
}
# Joining attribute sets.
nix-repl> { a = 1; } // { b = 2; }
{
  a = 1;
  b = 2;
}


# List, elements are separated by space.
nix-repl> [ 1 2 ]
[
  1
  2
]
# Joining lists.
nix-repl> [ 1 ] ++ [ 2 ]
[
  1
  2
]

# Simple function with argument.
nix-repl> f = a: a + 2
nix-repl> f 4
6

# Function taking an attribute set (? for default args).
nix-repl> f = { a, b ? 2 }: a + b
nix-repl> f { a = 1; b = 2; }
3
nix-repl> f { b = 1; a = 2; }
3
nix-repl> f { a = 1; }
3

# Function taking an attribute set w/o explicitly naming each input.
# Here, inp will bind to the whole input attribute set.
nix-repl> f = { a, b, ... } @ inp : a + inp.b + inp.c
nix-repl> f { b = 1; a = 2; c = 3; }
6

# Defining local variables with "let .. in".
nix-repl> let x = 1; in x + 3
4
nix-repl> x
error: undefined variable 'x'

# Let .. in expression returning an attribute set (handy when defining derivations).
nix-repl> let x = "abc"; in { name = "bar ${x}"; }
{ name = "bar abc"; }

# With to bring names into scope for the next expression.
nix-repl> x = { a = 1; b = 2; }
nix-repl> with x; [ a b ]
[
  1
  2
]

# Show all builtin functions.
nix-repl> builtins
{
  abort = «primop abort»;
  add = «primop add»;
  ..

# Factoring out code into multiple files.
# default.nix
#   { x, y }: {
#       a = x + 10;
#       b = y + 20;
#   }
#
# Import nix file by name ..
nix-repl> myfn = import ./default.nix
# .. or use short form for default.nix.
nix-repl> myfn = import ./.
nix-repl> myfn { x = 1; y = 2; }
{ a = 11; b = 22; }
nix-repl> x = 10
# 'inherit x;' is equal to 'x = x;' in the following.
nix-repl> myfn { inherit x ; y = 2; }
{ a = 20; b = 22; }
```

> The `import` builtin reads a nix expression from a file and
> evaluates it to return its value. For example `import ./default.nix
> { x = 1; y = 2; }` is equivalent to `(import ./foo.nix) { x = 1; y =
> 2; }`. First `import` reads and evaluates the nix expression in
> `foo.nix`, which returns the function (the value), which is then
> called with the attribute set as input.

### access flakes in the repl

```
# Access a flake inside repl.
nix-repl> pkgs = builtins.getFlake "nixpgs"
nix-repl> pkgs.legacyPackages.x86_64-linux.stdenvNoCC.mkDerivation
nix-repl> pkgs.legacyPackages.x86_64-linux.llvm  # tab
pkgs.legacyPackages.x86_64-linux.llvm                 pkgs.legacyPackages.x86_64-linux.llvmPackages_git
pkgs.legacyPackages.x86_64-linux.llvm-manpages        pkgs.legacyPackages.x86_64-linux.llvmPackages_latest
pkgs.legacyPackages.x86_64-linux.llvmPackages         pkgs.legacyPackages.x86_64-linux.llvm_12

# Alternatively flakes can be put into the lookup path.
#   arg:  nix repl -I nixpkgs=flake:nixpkgs
#   env:  NIX_PATH=nixpkgs=flake:nixpkgs
#   conf: extra-nix-path = nixpkgs=flake:nixpkgs

# Print the current nix path.
nix-repl> :p builtins.nixPath
```

### type example

The following gives an example how to read the type definitions commonly found
in the documentation. For that the type of the
[`nixpkgs.lib.genAttrs`][nixpkgs-genattrs] function is explored.
```
genAttrs :: [ String ] -> (String -> Any) -> AttrSet

# genAttrs is the name of the function.
# It takes as arguments
# - a list of strings - [ Strings ]
# - and a function which maps strings to anything - (String -> Any)
# The function then returns an attribute set.
```

## References

- [nix installable][nix-installable]
- [nix flakeref][nix-flakeref]
- [nix flake][nix-flake]
- [nix store][nix-store]
- [nix profile][nix-profile]
- [nix conf][nix-conf]
- [`builtin.derivation`][builtin-drv]
- [`nixpkgs.stdenv`][nixpkgs-stdenv]
- [`nixpkgs.mkShell`][nixpkgs-mkshell]
- [nix pills][nix-pills]
- [nixpkgs search][nixpkgs-search]


[nix-conf]: https://nix.dev/manual/nix/latest/command-ref/conf-file
[nix-conf-cmds]: https://nix.dev/manual/nix/latest/command-ref/conf-file.html#conf-experimental-features
[nix-search]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-search
[nix-profile]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-profile
[nix-flake]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-flake
[nix-flakeref]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-flake#flake-references
[nix-store]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-store
[nix-installable]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix#installables
[nix-run]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-run
[nix-shell]: https://nix.dev/manual/nix/latest/command-ref/new-cli/nix3-env-shell

[builtin-drv]: https://nix.dev/manual/nix/latest/language/derivations
[nixpkgs-stdenv]: https://nixos.org/manual/nixpkgs/stable/#chap-stdenv
[nixpkgs-nixdebug]: https://nixos.org/manual/nixpkgs/stable/#var-stdenv-NIX_DEBUG
[nixpkgs-mkshell]: https://nixos.org/manual/nixpkgs/stable/#sec-pkgs-mkShell
[nixpkgs-mkshell-nocc]: https://nixos.org/manual/nixpkgs/stable/#sec-pkgs-mkShell-variants
[nixpkgs-search]: https://search.nixos.org/packages
[nixpkgs-genattrs]: https://nixos.org/manual/nixpkgs/stable/#function-library-lib.attrsets.genAttrs

[nix-pills]: https://nixos.org/guides/nix-pills/
