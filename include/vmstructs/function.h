#ifndef QUANTUM_INCLUDE_VMSTRUCTS_FUNCTION_
#define QUANTUM_INCLUDE_VMSTRUCTS_FUNCTION_

#include <QtGlobal>
#include <QVector>

#include <vmstructs/constant.h>

struct Function {
    QVector<Constant> constants;
    QVector<QString> symbols;
    QVector<quint8> bytecodes;
};

#endif
