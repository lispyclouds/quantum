#include <vm.h>

void VM::printToStdOut(void* content, qint8 type) {
    static void* type_table[] = {
        &&INT,   // 0
        &&FLOAT, // 1
        &&STR,   // 2
        &&BOOL   // 3
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

void VM::run(QVector<Constant> constants, QVector<QString> symbols, QVector<quint8> bytecodes, QHash<QString, SymData> symbolTable) {
    QHash<QString, SymData> calleeSymTab;
    QStack<StackFrame> bytecodeStack;
    qint8 index;
    int sp = 0;
    SymData data;
    Function f;
    StackFrame frame, aux_frame;
    bool b_val = true;
    void* result;
    double *legRoom;

    bytecodes.push_back(0);

    static void* dispatch_table[] = {
        &&HALT,   // 0
        &&ICONST, // 1
        &&FCONST, // 2
        &&SCONST, // 3
        &&BOOL_T, // 4
        &&BOOL_F, // 5
        &&IADD,   // 6
        &&ISUB,   // 7
        &&IMUL,   // 8
        &&IDIV,   // 9
        &&FADD,   // 10
        &&FSUB,   // 11
        &&FMUL,   // 12
        &&FDIV,   // 13
        &&I2F,    // 14
        &&SET,    // 15
        &&LOAD,   // 16
        &&PRINT,  // 17
        &&SCAT,   // 18
        &&FUNC,   // 19
        &&CALL,   // 20
        &&RET,    // 21
        &&SETIP,  // 22
        &&SETFP,  // 23
        &&SETSP,  // 24
        &&SETBP   // 25
    };

    #define DISPATCH() goto *dispatch_table[FETCH()]
    #define FETCH() bytecodes[sp++]

    DISPATCH();

    RET:
        this->functionReturnFrame = bytecodeStack.pop();
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

    FUNC:
        index = FETCH();
        bytecodeStack.push(this->makeFrameOf(&functions[index], FUNCTION, true));
        DISPATCH();

    CALL:
        index = FETCH();
        f = *(Function *) symbolTable[symbols[index]].value;
        f.constants.append(constants);
        f.symbols.append(symbols);
        this->run(f.constants, f.symbols, f.bytecodes, calleeSymTab);
        bytecodeStack.push(this->functionReturnFrame);
        calleeSymTab.clear();
        DISPATCH();

    SETIP:
        data.type = INT;
    SETFP:
        data.type = FLOAT;
    SETSP:
        data.type = STRING;
    SETBP:
        data.type = BOOL;
        index = FETCH();
        data.value = constants[FETCH()].data;
        calleeSymTab[symbols[index]] = data;
        DISPATCH();
}
