/* SteelWM, a dynamic tiling window manager based on DWM
 * Copyright (C) 2025  Aceroph
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "X11/X.h"
#include "strings.h"
#include "toml.h"
#include <X11/keysym.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/config.h"
#include "include/steelwm.h"
#include "include/util.h"
#include "include/vector.h"

toml_table_t *config;

cvector_vector_type(Key) keybinds = NULL;
cvector_vector_type(Rule) rules = NULL;
cvector_vector_type(char *) tags = NULL;
cvector_vector_type(Button) buttons = NULL;
cvector_vector_type(char *) colors = NULL;
cvector_vector_type(char *) fonts = NULL;
cvector_vector_type(Layout) layouts = NULL;

unsigned int borderpx = 1; /* border pixel of windows */
unsigned int snap = 32;    /* snap pixel */
int showbar = 1;           /* 0 means no bar */
int topbar = 1;            /* 0 means bottom bar */

/* layout(s) */
float mfact = 0.55;     /* factor of master area size [0.05..0.95]*/
int nmaster = 1;        /* number of clients in master area */
int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

// private function definitions
KeySym map_key(const char *keybind);
unsigned int get_modkey(char *keybind);
void (*map_function(const char *function))(const Arg *arg);

void config_load() {
  char configpath[256];
  char errbuf[200];
  struct passwd *pw = getpwuid(getuid());

  if (pw == NULL) {
    die("failed to get the home directory");
  }

  snprintf(configpath, sizeof(configpath), "%s/.config/steelwm/config.toml",
           pw->pw_dir);

  FILE *fp = fopen(configpath, "r");
  if (!fp) {
    die("failed to open file at %s", configpath);
  }
  config = toml_parse_file(fp, errbuf, sizeof(errbuf));
  fclose(fp);

  if (!config) {
    die("failed to parse config: %s", errbuf);
  }
}

void config_get_keybinds() {
  toml_table_t *keybinds_config = toml_table_in(config, "keybinds");
  if (!keybinds_config) {
    die("no keybinds defined in configuration");
  }

  for (int i = 0;; i++) {
    char *keybind_name = (char *)toml_key_in(keybinds_config, i);
    if (!keybind_name)
      break;
    toml_datum_t bind = toml_string_in(keybinds_config, keybind_name);
    const char *function_name = strtok(bind.u.s, " ");

    int j = 0;
    char **args = calloc(j, sizeof(char *));
    char *arg = strtok(NULL, " ");
    while (arg != NULL) {
      char **newargs = realloc(args, sizeof(char *) * ++j);
      if (newargs == NULL) {
        die("failed to allocate more memory for keybind arguments");
      }
      args = newargs;
      args[j] = arg;
      arg = strtok(NULL, " ");
    }

    unsigned int modkey = get_modkey(keybind_name);
    KeySym key = map_key(keybind_name);
    void (*function)(const Arg *) = map_function(function_name);

    // TODO: Map args to type
    Key keybind = (Key){modkey, key, function, {.v = args}};
    cvector_push_back(keybinds, keybind);
  }
}

KeySym map_key(const char *keybind) {
  if (keybind == NULL) {
    die("missing keybind");
  }

  if (!strcasecmp(keybind, "space")) {
    return XK_space;
  }
  if (!strcasecmp(keybind, "enter") || !strcasecmp(keybind, "return")) {
    return XK_Return;
  }
  if (!strcasecmp(keybind, "delete"))
    return XK_Delete;
  if (!strcasecmp(keybind, "print")) {
    return XK_Print;
  }

  if (strlen(keybind) == 1) {
    int ascii = (int)keybind[0];
    if ((21 <= ascii && ascii <= 64) || (91 <= ascii && ascii <= 126)) {
      return ascii;
    }
  }

  die("invalid keybind %s", keybind);
  return 0;
}

unsigned int get_modkey(char *keybind) {
  unsigned int mod = 0;
  unsigned int shift = 0;
  unsigned int ctrl = 0;

  char *nonmodkey = NULL;

  char *key_t = strtok(keybind, "+");
  while (key_t != NULL) {
    if (!strcasecmp(key_t, "mod1") || !strcasecmp(key_t, "alt"))
      mod = Mod1Mask;
    else if (!strcasecmp(key_t, "mod2") || !strcasecmp(key_t, "numlock"))
      mod = Mod2Mask;
    else if (!strcasecmp(key_t, "mod3"))
      mod = Mod3Mask;
    else if (!strcasecmp(key_t, "mod4") || !strcasecmp(key_t, "super"))
      mod = Mod4Mask;
    else if (!strcasecmp(key_t, "mod5") || !strcasecmp(key_t, "altgr"))
      mod = Mod5Mask;
    else if (!strcasecmp(key_t, "shift"))
      shift = ShiftMask;
    else if (!strcasecmp(key_t, "control") || !strcasecmp(key_t, "ctrl"))
      ctrl = ControlMask;
    else
      nonmodkey = key_t;
    key_t = strtok(NULL, "+");
  }
  size_t size = strlen(nonmodkey);
  memcpy(keybind, nonmodkey, size);
  keybind[size] = '\0';

  return mod | shift | ctrl;
}

void (*map_function(const char *function))(const Arg *arg) {
  printf("funcname: %s\n", function);
  if (!strcasecmp(function, "spawn")) {
    return spawn;
  }
  if (!strcasecmp(function, "workspace")) {
    return focusstack;
  }
  if (!strcasecmp(function, "incnmaster")) {
    return incnmaster;
  }
  if (!strcasecmp(function, "setmfact")) {
    return setmfact;
  }
  if (!strcasecmp(function, "zoom")) {
    return zoom;
  }
  if (!strcasecmp(function, "view")) {
    return view;
  }
  if (!strcasecmp(function, "kill")) {
    return killclient;
  }
  if (!strcasecmp(function, "layout")) {
    return setlayout;
  }
  if (!strcasecmp(function, "toggle_floating")) {
    return togglefloating;
  }
  if (!strcasecmp(function, "tag")) {
    return tag;
  }
  if (!strcasecmp(function, "monitor")) {
    return focusmon;
  }
  if (!strcasecmp(function, "tag_monitor")) {
    return tagmon;
  }
  if (!strcasecmp(function, "quit")) {
    return quit;
  }

  die("unknown function %s", function);
}

int main() {
  config_load();

  config_get_keybinds();
  Key cmp = keybinds[0];
  Key test = {Mod1Mask, XK_Print, spawn, {.v = {"flameshot", NULL}}};
  if (test.mod == cmp.mod) {
    printf("same modkey\n");
  }
  if (test.keysym == cmp.keysym) {
    printf("same key\n");
  }
  if (test.func == cmp.func) {
    printf("same function\n");
  }
  if (test.arg.v == cmp.arg.v) {
    printf("same args\n");
  }

  toml_free(config);
  return 0;
}
