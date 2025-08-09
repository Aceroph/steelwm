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
    rev = "d33267fc62a84d75028e667c8cd184764f20c5bc";
    sha256 = "sha256-74btgds32SYD71Ba9Rya1eh3KqELkmFNwmySadc/vGY=";
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
