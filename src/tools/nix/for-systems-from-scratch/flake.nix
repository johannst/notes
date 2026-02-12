{
  description = "a flake for multiple systems from scratch";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      # "forSystems" is a function, which takes as argument a function.
      # It then calls the function passed for each value in the list "systems",
      # and passes the nixpkgs imported for the given system.
      # Finally it returns an attribute set where each key is the system in the
      # "systems" list and the value is what is returned from the invoked user
      # function.
      #
      # The pseudo code below shows the output.
      # {
      #   systems[0] = f pkgs;
      #   systems[1] = f pkgs;
      #   ...
      # }
      #
      # The "listToAttrs" builtin turns a list of name / value pairs (attribute
      # sets) into an attribute set.
      # > listToAttrs [ { name = "A"; value = "a"; } ... ] -> { A = "a"; ... }
      #
      # The "map" builtin takes a function and a list, then calls the function
      # on each list value and replaces the value with the result of the function.
      # > map (v: "m${v}") [ "a" "b" ... ] -> [ "ma" "mb" ... ]
      forSystems =
        f:
        builtins.listToAttrs (
          builtins.map (system: {
            name = system;
            value = f (import nixpkgs { inherit system; });
          }) systems
        );
    in
    {
      formatter = forSystems (pkgs: pkgs.nixfmt);

      devShells = forSystems (pkgs: {
        default = pkgs.mkShell {
          packages = [
            pkgs.strace
          ];
        };
      });
    };
}
