#include "mPrintf.h"

int mPrintf(const char *format, ...) {
    va_list args;
    int done;

    va_start(args, format);

    done = vprintf(format, args);

    va_end(args);

    return done;
}