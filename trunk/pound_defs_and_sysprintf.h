/*
	Yellowcot 1.2.4, released 2012-04-05

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

#define VERSION "1.2.4"
#define MINWIDTH 500
#define STRLEN 1000
#define MINHEIGHT 300
#define IMAGEBORDER 12
#define EDIT_TABLE_IMG_HEIGHT 50
#define TMPDIR "/var/tmp/yellowcot_quiz"
#define EXPORT_DPI 300
#define EXPORT_PAGE_W 8.5
#define EXPORT_PAGE_H 11
#define EXPORT_PAGE_ROWS 5
#define EXPORT_PAGE_BORDER 200
#define EXPORT_BLACK_BORDER_W 8
#define EXPORT_WHITE_BORDER_W 8
#define EXPORT_FONT_SIZE 64

inline void sysprintf(const char *fmt, ...) {
	char str[STRLEN];
	memset(str, 0, STRLEN);
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	system(str);
	va_end(argptr);
}