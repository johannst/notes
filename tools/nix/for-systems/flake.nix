{
  description = "a basic flake for multiple systems";

  inputs.nixpkgs.url = "nixpkgs";

  outputs =
    { self, nixpkgs }:
    let
      # forSystems is a function which takes a lambda (String -> Any).
      # genAttrs will call the lambda for each list element and finally return
      # an attribute set where the keys are the values in the list and the
      # values are the return values from the lambda invocation.
      forSystems = nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
      ];

      # pkgs is a function which takes a system as argument and imports the
      # nixpkgs for the given system.
      pkgs = system: import nixpkgs { inherit system; };
    in
    {
      # Generate formatter.${system} attribute set for each supported system.
      formatter = forSystems (system: (pkgs system).nixfmt);

      # Generate devShells.${system}.default attribute set for each supported
      # system.
      devShells = forSystems (system: {
        default =
          # The with statement brings the result of calling (pkgs system) into
          # scope for the next expression.
          with (pkgs system);
          mkShell {
            packages = [
              strace
            ]
            # An example how to customize per system.
            ++ nixpkgs.lib.optionals (system == "x86_64-linux") [ gdb ]
            ++ nixpkgs.lib.optionals (system == "aarch64-linux") [ lldb ];
          };
      });
    };
}
