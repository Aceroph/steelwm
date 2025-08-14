{
  config,
  lib,
  pkgs,
  ...
}:

let
  cfg = config.xsession.windowManager.steelwm;
in
{
  options.xsession.windowManager.steelwm = {
    enable = lib.mkEnableOption "Enable SteelWM";
    package = lib.mkOption {
      type = lib.types.package;
      default = pkgs.callPackage ./default.nix { inherit pkgs; };
    };
  };

  config = lib.mkIf cfg.enable {
    services.xserver.windowManager.session = lib.singleton {
      name = "steelwm";
      start = ''
        export _JAVA_AWT_WM_NONREPARENTING=1
        steelwm &
        waitPID=$!
      '';
    };

    environment.systemPackages = [ cfg.package ];
  };
}
