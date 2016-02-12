#include <iostream>

#include <QtCore/QCoreApplication>

#include <quantum_constants.h>

using namespace std;

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    cout << "Quantum Version "
         << QUANTUM_VERSION
         << endl
         << "compiled with Qt version "
         << qVersion()
         << endl;

    return 0;
}
