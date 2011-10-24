#include <stdarg.h>

#define VERSION "1.2.0"
#define STRLEN 1000
#define MINWIDTH 500
#define MINHEIGHT 300
#define IMAGEBORDER 12
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