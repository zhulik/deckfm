#include <QJSEngine>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>

#include <iostream>

#include <QDebug>

QString readFile(QString const &path) {
    QFile f(path);

    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        throw std::runtime_error(QString("Cannot read file %1").arg(path).toLocal8Bit());
    }

    return    QTextStream(&f).readAll();
}

int main(int argc, char *argv[])
{

    if (argc < 2) {
        qDebug() << QString("Usage: %1 [json or vdf file]");
        exit(1);
    }

    QCoreApplication app(argc, argv);

    QJSEngine myEngine;
    auto vdfModule =  myEngine.importModule(QCoreApplication::applicationDirPath() + "/vdf.js");

    if (vdfModule.isError()) {
        qDebug() << "Cannot load vdf.js:" << vdfModule.property("lineNumber").toInt()  << vdfModule.toString();
        exit(1);
    }

    auto filePath = argv[1];
    auto inputFile = readFile(filePath);

    auto suffix = QFileInfo(filePath).completeSuffix();

    QString output;

    if (suffix == "json") {
        auto parse = vdfModule.property("fromJSONString");
        QJSValueList args;
        args << inputFile;
        output =  parse.call(args).toString();
    } else if (suffix == "vdf") {
        auto parse = vdfModule.property("toJSONString");
        QJSValueList args;
        args << inputFile;
        output =  parse.call(args).toString();
    } else {
        qDebug() << "Unknown suffix:" << suffix;
        exit(1);
    }

    std::wcout << output.toStdWString().c_str() << std::endl;


}
