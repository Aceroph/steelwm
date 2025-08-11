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
#include "include/common.h"

#include <X11/keysym.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum COMMAND_TYPE map_command_type(const char *string) {
  if (!strcasecmp(string, "spawn")) {
    return SPAWN;
  }
  if (!strcasecmp(string, "workspace")) {
    return WORKSPACE;
  }
  if (!strcasecmp(string, "incnmaster")) {
    return INCNMASTER;
  }
  if (!strcasecmp(string, "setmfact")) {
    return SETMFACT;
  }
  if (!strcasecmp(string, "zoom")) {
    return ZOOM;
  }
  if (!strcasecmp(string, "view")) {
    return VIEW;
  }
  if (!strcasecmp(string, "kill")) {
    return KILL;
  }
  if (!strcasecmp(string, "layout")) {
    return LAYOUT;
  }
  if (!strcasecmp(string, "toggle_floating")) {
    return FLOATING;
  }
  if (!strcasecmp(string, "monitor")) {
    return MONITOR;
  }
  if (!strcasecmp(string, "tag_monitor")) {
    return TAGMON;
  }
  if (!strcasecmp(string, "quit")) {
    return QUIT;
  }

  die("unknown function %s", string);
  return 0;
}

unsigned int map_modkey(char *string) {
  unsigned int mod = 0;
  unsigned int shift = 0;
  unsigned int ctrl = 0;

  char *nonmodkey = NULL;

  char *key_t = strtok(string, "+");
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
  memcpy(string, nonmodkey, size);
  string[size] = '\0';

  return mod | shift | ctrl;
}

KeySym map_key(const char *string) {
  if (string == NULL) {
    die("missing string");
  }

  if (!strcasecmp(string, "space")) {
    return XK_space;
  }
  if (!strcasecmp(string, "enter") || !strcasecmp(string, "return")) {
    return XK_Return;
  }
  if (!strcasecmp(string, "delete"))
    return XK_Delete;
  if (!strcasecmp(string, "print")) {
    return XK_Print;
  }

  if (strlen(string) == 1) {
    int ascii = (int)string[0];
    if ((21 <= ascii && ascii <= 64) || (91 <= ascii && ascii <= 126)) {
      return ascii;
    }
  }

  die("invalid keybind %s", string);
  return 0;
}

void die(const char *fmt, ...) {
  va_list ap;
  int saved_errno;

  saved_errno = errno;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  if (fmt[0] && fmt[strlen(fmt) - 1] == ':')
    fprintf(stderr, " %s", strerror(saved_errno));
  fputc('\n', stderr);

  exit(1);
}

void *ecalloc(size_t nmemb, size_t size) {
  void *p;

  if (!(p = calloc(nmemb, size)))
    die("calloc:");
  return p;
}
