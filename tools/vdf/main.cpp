#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

#include <iostream>

#include "QSteamInput/vdfparser.h"

QString readFile(QString const &path) {
  QFile f(path);

  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    throw std::runtime_error(QString("Cannot read file %1").arg(path).toLocal8Bit());
  }

  return QTextStream(&f).readAll();
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    qDebug() << QString("Usage: %1 [json or vdf file]");
    exit(1);
  }

  QCoreApplication app(argc, argv);

  VDFParser parser;

  auto filePath = argv[1];
  auto inputFile = readFile(filePath);

  auto suffix = QFileInfo(filePath).completeSuffix();

  QString output;

  if (suffix == "json") {
    auto json = QJsonDocument::fromJson(inputFile.toUtf8()).object();
    output = parser.stringify(json);
  } else if (suffix == "vdf") {
    output = QJsonDocument(parser.parse(inputFile)).toJson(QJsonDocument::Indented);
  } else {
    qDebug() << "Unknown suffix:" << suffix;
    exit(1);
  }

  std::wcout << output.toStdWString().c_str() << std::endl;
}
