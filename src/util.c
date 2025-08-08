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
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/util.h"

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
