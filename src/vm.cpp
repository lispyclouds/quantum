#include <vm.h>

void VM::printToStdOut(void* content, qint8 type) {
    static void* type_table[] = {
        &&INT,   // 0
        &&FLOAT, // 1
        &&STR,   // 2
        &&BOOL   // 3
    };

    static void* bool_values[] = {
        &&FALSE,
        &&TRUE
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
        goto *bool_values[*(bool *) content];

    TRUE:
        QTextStream(stdout) << "true" << endl;
        return;

    FALSE:
        QTextStream(stdout) << "false" << endl;
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
        &&SETBP,  // 25
        &&ILT,    // 26
        &&ILE,    // 27
        &&IGT,    // 28
        &&IGE,    // 29
        &&IEQ,    // 30
        &&FLT,    // 31
        &&FLE,    // 32
        &&FGT,    // 33
        &&FGE,    // 34
        &&FEQ,    // 35
        &&SLT,    // 36
        &&SLE,    // 37
        &&SGT,    // 38
        &&SGE,    // 39
        &&SEQ,    // 40
        &&BEQ     // 41
    };

    #define DISPATCH() goto *dispatch_table[FETCH()]
    #define FETCH() bytecodes[sp++]
    #define POP2() frame = bytecodeStack.pop(); aux_frame = bytecodeStack.pop()

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
        POP2();
        result = ValueManip::add((qint64 *) aux_frame.content, (qint64 *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    ISUB:
        POP2();
        result = ValueManip::sub((qint64 *) aux_frame.content, (qint64 *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    IMUL:
        POP2();
        result = ValueManip::mul((qint64 *) aux_frame.content, (qint64 *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    IDIV:
        POP2();
        result = ValueManip::div((qint64 *) aux_frame.content, VM::checkDivideByZero((qint64 *) frame.content));
        bytecodeStack.push(this->makeFrameOf(result, INT));
        DISPATCH();

    FADD:
        POP2();
        result = ValueManip::add((double *) aux_frame.content, (double *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    FSUB:
        POP2();
        result = ValueManip::sub((double *) aux_frame.content, (double *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    FMUL:
        POP2();
        result = ValueManip::mul((double *) aux_frame.content, (double *) frame.content);
        bytecodeStack.push(this->makeFrameOf(result, FLOAT));
        DISPATCH();

    FDIV:
        POP2();
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
        POP2();
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
        f.constants += constants;
        f.symbols += symbols;
        this->run(f.constants, f.symbols, f.bytecodes, calleeSymTab);
        bytecodeStack.push(this->functionReturnFrame);
        calleeSymTab.clear();
        DISPATCH();

    SETIP:
        data.type = INT;
        goto pvalue;
    SETFP:
        data.type = FLOAT;
        goto pvalue;
    SETSP:
        data.type = STRING;
        goto pvalue;
    SETBP:
        data.type = BOOL;
    pvalue:
        index = FETCH();
        data.value = constants[FETCH()].data;
        calleeSymTab[symbols[index]] = data;
        DISPATCH();

    ILT:
        POP2();
        result = ValueManip::compare_lt((qint64 *) aux_frame.content, (qint64 *) frame.content);
        goto pushbool;
    ILE:
        POP2();
        result = ValueManip::compare_le((qint64 *) aux_frame.content, (qint64 *) frame.content);
        goto pushbool;
    IGT:
        POP2();
        result = ValueManip::compare_gt((qint64 *) aux_frame.content, (qint64 *) frame.content);
        goto pushbool;
    IGE:
        POP2();
        result = ValueManip::compare_ge((qint64 *) aux_frame.content, (qint64 *) frame.content);
        goto pushbool;
    IEQ:
        POP2();
        result = ValueManip::compare_eq((qint64 *) aux_frame.content, (qint64 *) frame.content);
        goto pushbool;
    FLT:
        POP2();
        result = ValueManip::compare_lt((double *) aux_frame.content, (double *) frame.content);
        goto pushbool;
    FLE:
        POP2();
        result = ValueManip::compare_le((double *) aux_frame.content, (double *) frame.content);
        goto pushbool;
    FGT:
        POP2();
        result = ValueManip::compare_gt((double *) aux_frame.content, (double *) frame.content);
        goto pushbool;
    FGE:
        POP2();
        result = ValueManip::compare_ge((double *) aux_frame.content, (double *) frame.content);
        goto pushbool;
    FEQ:
        POP2();
        result = ValueManip::compare_eq((double *) aux_frame.content, (double *) frame.content);
        goto pushbool;
    SLT:
        POP2();
        result = ValueManip::compare_lt((QString *) aux_frame.content, (QString *) frame.content);
        goto pushbool;
    SLE:
        POP2();
        result = ValueManip::compare_le((QString *) aux_frame.content, (QString *) frame.content);
        goto pushbool;
    SGT:
        POP2();
        result = ValueManip::compare_gt((QString *) aux_frame.content, (QString *) frame.content);
        goto pushbool;
    SGE:
        POP2();
        result = ValueManip::compare_ge((QString *) aux_frame.content, (QString *) frame.content);
        goto pushbool;
    SEQ:
        POP2();
        result = ValueManip::compare_eq((QString *) aux_frame.content, (QString *) frame.content);
        goto pushbool;
    BEQ:
        POP2();
        result = ValueManip::compare_eq((bool *) aux_frame.content, (bool *) frame.content);
    pushbool:
        bytecodeStack.push(this->makeFrameOf(result, BOOL));
        DISPATCH();
}
