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
    package = lib.mkOption {
      type = lib.types.package;
      default = pkgs.callPackage ./default.nix { inherit pkgs; };
    };
  };

  config = lib.mkIf cfg.enable {
    home.packages = [ steelwm ];

    xsession.windowManager.command = "${steelwm}/bin/steelwm";
  };
}
