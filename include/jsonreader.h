#ifndef QUANTUM_INCLUDE_VMSTRUCTS_JSONREADER_
#define QUANTUM_INCLUDE_VMSTRUCTS_JSONREADER_

#include <iostream>

#include <QByteArray>
#include <QFile>
#include <QtGlobal>
#include <QVector>
#include <gc_cpp.h>
#include <rapidjson/document.h>

#include <vmstructs/constant.h>

using namespace std;
using namespace rapidjson;

void readJSON(QString fileName, QVector<Constant> &constants, QVector<QString> &symbols, QVector<quint8> &bytecodes) {
    QFile jsonFile(fileName);

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        cerr << "Couldn't open JSON file." << endl;
        exit(-2);
    }

    QByteArray array = jsonFile.readAll();
    jsonFile.close();

    const char *json = array.constData();
    Document document;

    char *buffer = new(GC) char[array.size()];
    memcpy(buffer, json, array.size());
    if (document.ParseInsitu(buffer).HasParseError()) {
        cerr << "JSON file contains errors" << endl;
        exit(-3);
    }

    void* ptr;
    Constant c;
    SizeType i;

    Value& arr = document["constants"];
    for (i = 0; i < arr.Size(); i++) {
        if (arr[i].IsInt())
            ptr = new(GC) qint64(arr[i].GetInt());
        else if (arr[i].IsDouble())
            ptr = new(GC) double(arr[i].GetDouble());
        else if (arr[i].IsBool())
            ptr = new(GC) bool(arr[i].GetBool());
        else if (arr[i].IsString())
            ptr = new(GC) QString(arr[i].GetString());

        c.data = ptr;
        constants.push_back(c);
    }

    arr = document["symbols"];
    for (i = 0; i < arr.Size(); i++) {
        symbols.push_back(QString(arr[i].GetString()));
    }

    arr = document["bytecodes"];
    for (i = 0; i < arr.Size(); i++) {
        bytecodes.push_back(arr[i].GetInt());
    }
}

#endif
