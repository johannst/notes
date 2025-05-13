# The flake is effectively an attribute set with a standard set of attributes.
{
  description = "bare bones (flake)";

  # Input(s) of the flake.
  # Each input attribute is passed into the output function below.
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  # The 'outputs' attribute is a function, accepting an attribute set with a
  # self reference and all the inputs specified above.
  # The function shall return an attribute set with different outputs.
  # There are standardized outputs for different commands such as:
  #   > nix build
  #   > nix develop
  outputs = { self, nixpkgs }: let
    # Define local variables with let .. in construct.
    system = "x86_64-linux";
    # Import another flake, passing an attribute set.
    # inherit system, is effectively system = system;
    pkgs = import nixpkgs { inherit system; };
  in
  # Return attribute set.
  {
    # Some output.
    moose = builtins.derivation {
      # -- Required attributes.
      inherit system;
      name = "bone";
      builder = "${pkgs.bash}/bin/bash";
      # -- Optional attributes.
      args = [ "-c" "echo hello > $out" ];
    };
  };
}
