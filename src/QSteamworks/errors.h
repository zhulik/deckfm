#pragma once

#include <QString>

#include <stdexcept>


namespace QSteamworks {
class Exception: public std::exception {
public:
    Exception(const QString &what) : m_what(what) {}

    char *what () { return m_what.toLocal8Bit().data(); }

protected:
    QString m_what;
};

class InitializationFailed: public Exception {
public:
    InitializationFailed(const QString &what) : Exception(what) {}
};
}
