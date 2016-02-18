#ifndef QUANTUM_INCLUDE_CORE_VALUEMANIP_
#define QUANTUM_INCLUDE_CORE_VALUEMANIP_

#include <iostream>

#include <QtGlobal>
#include <QString>
#include <gc_cpp.h>

using namespace std;

class ValueManip {
public:
    static void* add(qint64* d1, qint64* d2);
    static void* add(double* d1, double* d2);
    static void* sub(qint64* d1, qint64* d2);
    static void* sub(double* d1, double* d2);
    static void* mul(qint64* d1, qint64* d2);
    static void* mul(double* d1, double* d2);
    static void* div(qint64* d1, qint64* d2);
    static void* div(double* d1, double* d2);
};

#endif
