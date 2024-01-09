#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

__attribute__((noreturn)) void
    _abort(const char *msg, const char *file, unsigned line) {
    //! FIXME: page fault if stdout is invalid
    printf("%s:%d: %s\n", file, line, msg);
    exit(-1);
    while (1) {}
}
