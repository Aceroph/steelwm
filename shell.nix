let
  pkgs = import <nixpkgs> { };
in
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    xorg.libXinerama
    xorg.libX11.dev
    xorg.libXft
  ];

  packages = with pkgs; [
    cmake
  ];
}
