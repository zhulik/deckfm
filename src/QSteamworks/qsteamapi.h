#pragma once

#include "qobjectdefs.h"
#include <QObject>
#include <stdexcept>

#include <exception>
#include <stdexcept>

namespace QSteamworks{

class QSteamUtils;
class QSteamInput;

class QSteamAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSteamUtils* steamUtils READ steamUtils CONSTANT)
    Q_PROPERTY(QSteamInput* steamInput READ steamInput CONSTANT)
public:
    explicit QSteamAPI(QObject *parent = nullptr);
    virtual ~QSteamAPI();

    Q_INVOKABLE
        void runCallbacks() const;

    QSteamUtils *steamUtils() const;

    QSteamInput *steamInput();

signals:

private:
    QSteamUtils *m_steamUtils = nullptr;
    QSteamInput *m_steamInput = nullptr;
};
}
