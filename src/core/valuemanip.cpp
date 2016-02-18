#include <core/valuemanip.h>

void* ValueManip::add(qint64* d1, qint64* d2) {
    void* result = new(GC) qint64();
    *((qint64 *) result) = *(qint64 *) d1 + *(qint64 *) d2;

    return result;
}

void* ValueManip::add(double* d1, double* d2) {
    void* result = new(GC) double();
    *((double *) result) = *(double *) d1 + *(double *) d2;

    return result;
}

void* ValueManip::sub(qint64* d1, qint64* d2) {
    void* result = new(GC) qint64();
    *((qint64 *) result) = *(qint64 *) d1 - *(qint64 *) d2;

    return result;
}

void* ValueManip::sub(double* d1, double* d2) {
    void* result = new(GC) double();
    *((double *) result) = *(double *) d1 - *(double *) d2;

    return result;
}

void* ValueManip::mul(qint64* d1, qint64* d2) {
    void* result = new(GC) qint64();
    *((qint64 *) result) = *(qint64 *) d1 * *(qint64 *) d2;

    return result;
}

void* ValueManip::mul(double* d1, double* d2) {
    void* result = new(GC) double();
    *((double *) result) = *(double *) d1 * *(double *) d2;

    return result;
}

void* ValueManip::div(qint64* d1, qint64* d2) {
    void* result = new(GC) qint64();
    *((qint64 *) result) = *(qint64 *) d1 / *(qint64 *) d2;

    return result;
}

void* ValueManip::div(double* d1, double* d2) {
    void* result = new(GC) double();
    *((double *) result) = *(double *) d1 / *(double *) d2;

    return result;
}

void* ValueManip::add(QString* d1, QString* d2) {
    void* result = new(GC) QString();
    *((QString *) result) = *(QString *) d1 + *(QString *) d2;

    return result;
}
