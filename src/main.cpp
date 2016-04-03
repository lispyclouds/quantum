#include <iostream>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>

#include <jsonreader.h>
#include <quantum_constants.h>
#include <compiler/compiler.hpp>
#include <vm.h>
#include <vmstructs/constant.h>
#include <vmstructs/function.h>

using namespace std;

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("trost");
    QCoreApplication::setApplicationVersion(QUANTUM_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("The Trost VM and Runtime");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption jsonOption(QStringList() << "j" << "json", "Treat input file as compiled bytecode JSON file");
    parser.addOption(jsonOption);
    parser.addPositionalArgument("source.tr/json", "Specify the trost source/json file for quantum to crunch");

    parser.process(app);
    const QStringList args = parser.positionalArguments();

    if (args.size() != 1)
        parser.showHelp(-1);

    QVector<Constant> constants;
    QVector<QString> symbols;
    QVector<quint8> bytecodes;
    QVector<Function> functions;
    QHash<QString, SymData> symbolTable;

    if (parser.isSet(jsonOption))
        readJSON(args[0], constants, symbols, bytecodes, functions);
    else
        runFile(args[0].toStdString().c_str(), constants, symbols, bytecodes, functions);

    VM vm(functions);
    vm.run(constants, symbols, bytecodes, symbolTable);

    return 0;
}
