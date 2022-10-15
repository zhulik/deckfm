#pragma once

#include "qobjectdefs.h"
#include <QObject>

namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
public:
    explicit QSteamInput(QObject *parent = nullptr);
    virtual ~QSteamInput();

    Q_INVOKABLE
    void runFrame();
};
}
