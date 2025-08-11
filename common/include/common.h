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
#ifndef COMMON_H_
#define COMMON_H_

#include <X11/X.h>
#include <stdlib.h>

enum COMMAND_TYPE {
  FLOATING, // togglefloating
  INCNMASTER,
  KILL,    // killclient
  LAYOUT,  // setlayout
  MONITOR, // focusmon
  QUIT,
  SETMFACT,
  SPAWN,
  TAGMON,
  VIEW,
  WINDOW,    // focusstack
  WORKSPACE, // tag
  ZOOM,
};

enum COMMAND_TYPE map_command_type(const char *string);
unsigned int map_modkey(char *string);
KeySym map_key(const char *string);

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);

#endif // COMMON_H_
