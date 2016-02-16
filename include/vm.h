#ifndef QUANTUM_INCLUDE_VM_
#define QUANTUM_INCLUDE_VM_

#include <iostream>

#include <QtGlobal>
#include <QHash>
#include <QStack>
#include <QVector>

#include <core/valuemanip.h>
#include <vmstructs/constant.h>
#include <vmstructs/datatypes.h>
#include <vmstructs/stackframe.h>

using namespace std;

struct SymData {
    int type;
    void* value;
};

class VM {
public:
    VM(QVector<Constant> constants, QVector<QString> symbols, QVector<quint8> bytecodes);
    ~VM();

    void run();

private:
    QVector<Constant> constants;
    QHash<QString, SymData> symbolTable;
    QVector<quint8> bytecodes;
    QStack<StackFrame> bytecodeStack;
    QVector<QString> symbols;
    int sp;

    inline void printToStdOut(void* content, qint8 type);
    inline StackFrame makeFrameOf(void* content, qint8 dataType, bool callable);
    inline quint8 fetch();
};

#endif
