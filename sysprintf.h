#include <stdarg.h>

inline int sysprintf(int count,...) {
	va_list ap;
	int i, sum;
	va_start (ap, count);
	sum = 0;
	for (i = 0; i < count; i++)
		sum += va_arg (ap, int);
	va_end (ap);
	return sum;
}