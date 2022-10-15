#pragma once

#include <QJSEngine>
#include <QJsonObject>


class VDFParser
{
public:
    VDFParser(){
        auto vdfModule =  m_engine.importModule(":/resources/js/vdf.js");

        if (vdfModule.isError()) {
            throw std::runtime_error(QString("Cannot load vdf.js: %1 %2").arg(vdfModule.property("lineNumber").toInt())
                                     .arg(vdfModule.toString()).toLocal8Bit());
        }

        m_parse = vdfModule.property("parse");
        m_stringify = vdfModule.property("stringify");
    };

    QJsonObject parse(const QString &vdf) {
        QJSValueList args;
        args << vdf;
        return m_parse.call(args).toVariant().toJsonObject();
    };

    QString stringify(const QJsonObject &obj) {
        QJSValueList args;
        args << m_engine.toScriptValue(obj);
        return m_stringify.call(args).toString();
    };


private:
    QJSEngine m_engine;
    QJSValue m_parse;
    QJSValue m_stringify;

};

