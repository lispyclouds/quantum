#include <iostream>

#include <core/valuemanip.h>

using namespace std;

void* ValueManip::quantum_add(void* d1, void* d2, qint8 type) {
    static void* type_table[] = {
        &&INT,
        &&FLOAT,
        &&STR
    };
    void* result;

    goto *type_table[type];

    INT:
        result = new(GC) qint64();
        *((qint64 *) result) = *(qint64 *) d1 + *(qint64 *) d2;
        return result;

    FLOAT:
        result = new(GC) double();
        *((double *) result) = *(double *) d1 + *(double *) d2;
        return result;

    STR:
        result = new(GC) QString();
        *((QString *) result) = *(QString *) d1 + *(QString *) d2;
        return result;
}
