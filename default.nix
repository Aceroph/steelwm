{
  lib,
  pkgs,
  stdenv,
  ...
}:

stdenv.mkDerivation {
  pname = "steelwm";
  version = "1.0";
  src = ./.;

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
