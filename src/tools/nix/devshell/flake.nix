{
  description = "dev shell (flake)";

  inputs = { nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable"; };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        packages = [
          pkgs.zig
          pkgs.zls
        ];
        # https://github.com/NixOS/nixpkgs/issues/270415
        shellHook = ''
          unset ZIG_GLOBAL_CACHE_DIR
        '';
      };
    };
}
