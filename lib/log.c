#include "Common.h"
#include <stdarg.h>

void error(int status, int err, char* format, ...) {
    va_list args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    if(err) {
        frpintf(stderr, ": %s (%d) \n", strerror(err), err);
    }

    if(status) {
        exit(status);
    }
}