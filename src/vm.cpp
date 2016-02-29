#include <vm.h>

void VM::printToStdOut(void* content, qint8 type) {
    static void* type_table[] = {
        &&INT,   // 0x0
        &&FLOAT, // 0x1
        &&STR,   // 0x2
        &&BOOL   // 0x3
    };

    goto *type_table[type];

    INT:
        QTextStream(stdout) << *(qint64 *) content << endl;
        return;

    FLOAT:
        QTextStream(stdout) << *(double *) content << endl;
        return;

    STR:
        QTextStream(stdout) << *(QString *) content << endl;
        return;

    BOOL:
        QTextStream(stdout) << *(bool *) content << endl;
        return;
}

StackFrame VM::makeFrameOf(void* content, qint8 dataType, bool callable=false) {
    StackFrame frame;

    frame.content = content;
    frame.dataType = dataType;
    frame.callable = callable;

    return frame;
}

template <class T>
T* VM::checkDivideByZero(T* divisor) {
    static void* is_zero[] = {
        &&NOT_ZERO,
        &&ZERO
    };
    goto *is_zero[!(*divisor || 0)]; // Freaky Stuff huh :P ??

    NOT_ZERO:
        return divisor;

    ZERO:
        cerr << "Fatal Error: Division by ZERO." << endl;
        exit(-4);
}

void VM::run(QVector<Constant> constants, QVector<QString> symbols, QVector<quint8> bytecodes) {
    QHash<QString, SymData> symbolTable;
    QStack<StackFrame> bytecodeStack;
    qint8 index;
    int sp = 0;
    SymData data;
    StackFrame frame, aux_frame;
    bool b_val = true;
    void* result;
    double *legRoom;

    bytecodes.push_back(0);

    static void* dispatch_table[] = {
        &&HALT,
        &&ICONST,
        &&FCONST,
        &&SCONST,
        &&BOOL_T,
        &&BOOL_F,
        &&IADD,
        &&ISUB,
        &&IMUL,
        &&IDIV,
        &&FADD,
        &&FSUB,
        &&FMUL,
        &&FDIV,
        &&I2F,
        &&SET,
        &&LOAD,
        &&PRINT,
        &&SCAT
    };

    #define DISPATCH() goto *dispatch_table[FETCH()]
    #define FETCH() bytecodes[sp++]

    DISPATCH();

    HALT:
        return;

    ICONST:
        index = FETCH();
        bytecodeStack.push(this->makeFrameOf(constants[index].data, INT));
        DISPATCH();

    FCONST:
        index = FETCH();
        bytecodeStack.push(this->makeFrameOf(constants[index].data, FLOAT));
        DISPATCH();

    SCONST:
        index = FETCH();
        bytecodeStack.push(this->makeFrameOf(constants[index].data, STRING));
        DISPATCH();

    BOOL_T:
        bytecodeStack.push(this->makeFrameOf(&b_val, BOOL));
        DISPATCH();

    BOOL_F:
        b_val = false;
        bytecodeStack.push(this->makeFrameOf(&b_val, BOOL));
        DISPATCH();

    IADD:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::add((qint64 *) aux_frame.content, (qint64 *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    ISUB:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::sub((qint64 *) aux_frame.content, (qint64 *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    IMUL:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::mul((qint64 *) aux_frame.content, (qint64 *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    IDIV:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::div((qint64 *) aux_frame.content, VM::checkDivideByZero((qint64 *) frame.content));
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    FADD:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::add((double *) aux_frame.content, (double *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    FSUB:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::sub((double *) aux_frame.content, (double *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    FMUL:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::mul((double *) aux_frame.content, (double *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    FDIV:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::div((double *) aux_frame.content, VM::checkDivideByZero((double *) frame.content));
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    I2F:
        frame = bytecodeStack.pop();
        legRoom = new(GC) double();
        *legRoom = *(qint64 *) frame.content;
        bytecodeStack.push(this->makeFrameOf(legRoom, FLOAT));
        DISPATCH();

    SET:
        index = FETCH();
        frame = bytecodeStack.pop();
        data.type = frame.dataType;
        data.value = frame.content;
        symbolTable[symbols[index]] = data;
        DISPATCH();

    LOAD:
        index = FETCH();
        data = symbolTable[symbols[index]];
        bytecodeStack.push(this->makeFrameOf(data.value, data.type));
        DISPATCH();

    PRINT:
        frame = bytecodeStack.pop();
        this->printToStdOut(frame.content, frame.dataType);
        DISPATCH();

    SCAT:
        frame = bytecodeStack.pop();
        aux_frame = bytecodeStack.pop();
        result = ValueManip::add((QString *) aux_frame.content, (QString *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, STRING));
        DISPATCH();
}
