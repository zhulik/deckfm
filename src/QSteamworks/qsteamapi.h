#pragma once

#include "qobjectdefs.h"
#include <QObject>
#include <stdexcept>

#include <exception>
#include <stdexcept>

namespace QSteamworks{

class QSteamUtils;

class QSteamAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSteamUtils* steamUtils READ steamUtils CONSTANT)
public:
    explicit QSteamAPI(QObject *parent = nullptr);
    virtual ~QSteamAPI();

    Q_INVOKABLE
        void runCallbacks() const;

    QSteamUtils *steamUtils() const;

signals:

private:
    QSteamUtils *m_steamUtils = nullptr;
};
}
