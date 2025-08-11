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
#include "toml.h"
#include <X11/keysym.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/include/common.h"
#include "include/config.h"
#include "include/steelwm.h"
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

void (*map_function(enum COMMAND_TYPE command_type))(const Arg *arg);
void config_get_keybinds();
void config_get_tags();

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

  config_get_keybinds();
  config_get_tags();
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

    cvector_vector_type(char *) args = NULL;
    char *arg;
    do {
      arg = strtok(NULL, " ");
      cvector_push_back(args, arg);
    } while (arg != NULL);

    unsigned int modkey = map_modkey(keybind_name);
    KeySym key = map_key(keybind_name);
    void (*function)(const Arg *) =
        map_function(map_command_type(function_name));

    // TODO: Map args to type
    Key keybind = (Key){modkey, key, function, {.v = args}};
    cvector_push_back(keybinds, keybind);
  }
}

void config_get_tags() {
  toml_array_t *tags_config = toml_array_in(config, "workspaces");
  if (tags_config) {
    for (int i = 0;; i++) {
      toml_datum_t tag = toml_string_at(tags_config, i);
      if (!tag.ok)
        break;
      cvector_push_back(tags, tag.u.s);
    }
  } else {
    cvector_push_back(tags, "1");
    cvector_push_back(tags, "2");
    cvector_push_back(tags, "3");
    cvector_push_back(tags, "4");
    cvector_push_back(tags, "5");
    cvector_push_back(tags, "6");
    cvector_push_back(tags, "7");
    cvector_push_back(tags, "8");
    cvector_push_back(tags, "9");
  }
}

void (*map_function(enum COMMAND_TYPE command_type))(const Arg *arg) {
  switch (command_type) {
  case SPAWN:
    return spawn;
  case WORKSPACE:
    return tag;
  case INCNMASTER:
    return incnmaster;
  case SETMFACT:
    return setmfact;
  case ZOOM:
    return zoom;
  case VIEW:
    return view;
  case KILL:
    return killclient;
  case LAYOUT:
    return setlayout;
  case FLOATING:
    return togglefloating;
  case WINDOW:
    return focusstack;
  case TAGMON:
    return tagmon;
  case QUIT:
    return quit;
  case MONITOR:
    return focusmon;
  }
}

int main() {
  config_load();

  for (int i = 0; i < cvector_size(tags); i++) {
    printf("%d: %s\n", i, tags[i]);
  }

  toml_free(config);
  return 0;
}
