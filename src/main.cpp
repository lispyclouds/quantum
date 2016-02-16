#include <iostream>

#include <QCommandLineParser>
#include <QCoreApplication>

#include <jsonreader.h>
#include <quantum_constants.h>
#include <vm.h>
#include <vmstructs/constant.h>

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

    if (args.size() == 0)
        parser.showHelp(-1);

    QVector<Constant> constants;
    QVector<QString> symbols;
    QVector<quint8> bytecodes;

    readJSON(args[0], constants, symbols, bytecodes);

    VM vm(constants, symbols, bytecodes);
    vm.run();

    return 0;
}
