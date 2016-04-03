#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "trost.h"

void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
