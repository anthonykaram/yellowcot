/*
	Yellowcot 1.2.1, released 2012-01-12

	Copyleft 2012 Anthony Karam Karam

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdarg.h>

#define VERSION "1.2.1"
#define STRLEN 1000
#define MINWIDTH 500
#define MINHEIGHT 300
#define IMAGEBORDER 12
#define EDIT_TABLE_IMG_HEIGHT 50
#define TMPDIR "/var/tmp/yellowcot_quiz"

inline void sysprintf(const char *fmt, ...) {
	char str[STRLEN];
	memset(str, 0, STRLEN);
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	system(str);
	va_end(argptr);
}