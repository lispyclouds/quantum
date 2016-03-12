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
#include <vmstructs/function.h>

using namespace std;
using namespace rapidjson;

void readJSON(QString fileName, QVector<Constant> &constants, QVector<QString> &symbols, QVector<quint8> &bytecodes, QVector<Function> &functions) {
    QFile jsonFile(fileName);

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        cerr << "Couldn't open JSON file." << endl;
        exit(-2);
    }

    GC_enable_incremental();
    GC_INIT();

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

    void* ptr = NULL;
    Constant c;
    SizeType i, j;

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

    if (document.HasMember("functions")) {
        arr = document["functions"];
        Function f;

        for (i = 0; i < arr.Size(); i++) {
            Value& func_arr = arr[i]["constants"];

            for (j = 0; j < func_arr.Size(); j++) {
                if (func_arr[j].IsInt())
                    ptr = new(GC) qint64(func_arr[j].GetInt());
                else if (func_arr[j].IsDouble())
                    ptr = new(GC) double(func_arr[j].GetDouble());
                else if (func_arr[j].IsBool())
                    ptr = new(GC) bool(func_arr[j].GetBool());
                else if (func_arr[j].IsString())
                    ptr = new(GC) QString(func_arr[j].GetString());

                c.data = ptr;
                f.constants.push_back(c);
            }

            func_arr = arr[i]["symbols"];
            for (j = 0; j < func_arr.Size(); j++) {
                f.symbols.push_back(func_arr[j].GetString());
            }

            func_arr = arr[i]["bytecodes"];
            for (j = 0; j < func_arr.Size(); j++) {
                f.bytecodes.push_back(func_arr[j].GetInt());
            }

            functions.push_back(f);
            f.constants.clear();
            f.symbols.clear();
            f.bytecodes.clear();
        }
    }
}

#endif
