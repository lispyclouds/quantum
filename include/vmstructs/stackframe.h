#ifndef QUANTUM_INCLUDE_VMSTRUCTS_STACKFRAME_
#define QUANTUM_INCLUDE_VMSTRUCTS_STACKFRAME_

#include <QtGlobal>

struct StackFrame {
    void* content;
    qint8 dataType;
    bool callable;
};

#endif
