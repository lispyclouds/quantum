#ifndef QUANTUM_INCLUDE_VMSTRUCTS_JSONREADER_
#define QUANTUM_INCLUDE_VMSTRUCTS_JSONREADER_

#include <iostream>

#include <QByteArray>
#include <QFile>
#include <QtGlobal>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVector>

#include <vmstructs/constant.h>

using namespace std;

void readJSON(QString fileName, QVector<Constant> &constants, QVector<QString> &symbols, QVector<quint8> &bytecodes) {
    QFile json(fileName);

    if (!json.open(QIODevice::ReadOnly)) {
        cerr << "Couldn't open JSON file." << endl;
        exit(-2);
    }

    QByteArray data = json.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    if (doc.isNull()) {
        cerr << "Invalid JSON file." << endl;
        exit(-3);
    }

    const QJsonObject code = doc.object();
    QJsonArray arr;
    void* ptr;
    Constant c;
    int i;

    arr = code["constants"].toArray();
    for (i = 0; i < arr.size(); i++) {
        if (arr[i].isDouble()) {
            bool ok;
            double value = arr[i].toDouble();
            QString::number(value).toInt(&ok);

            if (ok)
                ptr = new qint64(value);
            else
                ptr = new double(value);
        }
        else if (arr[i].isBool())
            ptr = new bool(arr[i].toBool());
        else if (arr[i].isString())
            ptr = new QString(arr[i].toString());

        c.data = ptr;
        constants.push_back(c);
    }

    arr = code["symbols"].toArray();
    for (i = 0; i < arr.size(); i++) {
        symbols.push_back(arr[i].toString());
    }

    arr = code["bytecodes"].toArray();
    for (i = 0; i < arr.size(); i++) {
        bytecodes.push_back(arr[i].toInt());
    }
}

#endif
