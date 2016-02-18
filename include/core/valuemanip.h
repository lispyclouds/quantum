#ifndef QUANTUM_INCLUDE_CORE_VALUEMANIP_
#define QUANTUM_INCLUDE_CORE_VALUEMANIP_

#include <iostream>

#include <QtGlobal>
#include <QString>
#include <gc_cpp.h>

using namespace std;

class ValueManip {
public:
    static void* quantum_add(void* d1, void* d2, qint8 type);
};

#endif
