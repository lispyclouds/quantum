#include <cstdio>

#include <vmstructs/constant.h>
#include <vmstructs/function.h>

extern FILE* yyin;
void yyparse(void);

void runFile(const char* fileName, QVector<Constant> &constants, QVector<QString> &symbols, QVector<quint8> &bytecodes, QVector<Function> &functions) {
    if (!(yyin = fopen(fileName, "r"))) {
        perror(fileName);
        exit(1);
    }

    yyparse();
    fclose(yyin);
}
