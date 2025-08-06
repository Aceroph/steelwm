{
  description = "SteelWM, a dynamic tiling window manager based on DWM";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      packages.${system} = rec {
        steelwm = pkgs.callPackage ./default.nix { };
        default = steelwm;
      };
    };
}
