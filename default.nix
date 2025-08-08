{
  lib,
  pkgs,
  stdenv,
  ...
}:

stdenv.mkDerivation {
  pname = "steelwm";
  version = "0.1";
  src = pkgs.fetchFromGitHub {
    owner = "aceroph";
    repo = "steelwm";
    rev = "9c1bcf997bbfd4696a4b888a2de28c50302f5327";
    sha256 = "sha256-A28lPM6ci0JHsABRVS9UfHxSRirW9oRk/ZmOLqucnMY=";
  };

  nativeBuildInputs = with pkgs; [
    xorg.libXinerama
    xorg.libX11
    xorg.libXft
  ];

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin
    cp steelwm $out/bin/

    runHook postInstall
  '';

  meta = with lib; {
    homepage = "https://github.com/Aceroph/steelwm";
    description = "SteelWM, a dynamic tiling window manager based on DWM";
    license = licenses.gpl3Only;
    platforms = platforms.linux;
    mainProgram = "steelwm";
  };
}
