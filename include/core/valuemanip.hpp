#ifndef QUANTUM_INCLUDE_CORE_VALUEMANIP_
#define QUANTUM_INCLUDE_CORE_VALUEMANIP_

#include <iostream>

#include <QtGlobal>
#include <QString>
#include <gc_cpp.h>

using namespace std;

class ValueManip {
public:
    template <class T>
    static inline void* add(T* d1, T* d2) {
        void* result = new(GC) T();
        *((T *) result) = *(T *) d1 + *(T *) d2;

        return result;
    }

    template <class T>
    static inline void* sub(T* d1, T* d2) {
        void* result = new(GC) T();
        *((T *) result) = *(T *) d1 - *(T *) d2;

        return result;
    }

    template <class T>
    static inline void* mul(T* d1, T* d2) {
        void* result = new(GC) T();
        *((T *) result) = *(T *) d1 * *(T *) d2;

        return result;
    }

    template <class T>
    static inline void* div(T* d1, T* d2) {
        void* result = new(GC) T();
        *((T *) result) = *(T *) d1 / *(T *) d2;

        return result;
    }
};

#endif
