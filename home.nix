{
  config,
  lib,
  pkgs,
  ...
}:

let
  cfg = config.xsession.windowManager.steelwm;
  steelwm = cfg.package;
in
{
  options.xsession.windowManager.steelwm = {
    enable = lib.mkEnableOption "Enable SteelWM";
    package = lib.mkPackageOption pkgs "steelwm" { extraDescription = "Package to use"; };
  };

  config = lib.mkIf cfg.enable {
    home.packages = [ steelwm ];
  };
}
