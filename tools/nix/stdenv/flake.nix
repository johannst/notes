{
  description = "stdenv (flake)";

  inputs = { nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable"; };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      # Enable support for 'nix fmt'.
      formatter.${system} = pkgs.nixfmt;

      # This is the default output that will be build & run if no output is
      # specified, when running the following in the flakes directory.
      # > nix build
      # > nix run
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        # Either specify 'name' or 'pname & version'.
        # In the latter case, 'name' will be ${pname}-${version}.
        pname = "test";
        version = "0.1";

        # There are different builtins to also unpack or fetch tarballs.
        src = ./src;

        # The default builder of the stdenv defines many different phases that
        # can be overwritten.

        buildPhase = ''
          gcc -O2 -g -o test test.c
        '';

        installPhase = ''
          mkdir -p $out/bin
          cp test $out/bin
        '';
      };
    };
}
