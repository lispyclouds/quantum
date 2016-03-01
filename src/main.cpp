#include <iostream>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <gc_cpp.h>

#include <jsonreader.h>
#include <quantum_constants.h>
#include <vm.h>
#include <vmstructs/constant.h>
#include <vmstructs/function.h>

using namespace std;

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("quantum");
    QCoreApplication::setApplicationVersion(QUANTUM_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("Quantum Standalone: The VM for Trost");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source.json", "Specify the json file for quantum to crunch");

    parser.process(app);
    const QStringList args = parser.positionalArguments();

    if (args.size() != 1)
        parser.showHelp(-1);

    QVector<Constant> constants;
    QVector<QString> symbols;
    QVector<quint8> bytecodes;
    QVector<Function> functions;

    readJSON(args[0], constants, symbols, bytecodes, functions);

    GC_INIT();
    VM vm(functions);
    vm.run(constants, symbols, bytecodes);

    return 0;
}
