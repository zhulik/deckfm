#pragma once

#include "qobjectdefs.h"
#include <QObject>

namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
public:
    explicit QSteamInput(QSteamAPI *parent = nullptr);
    virtual ~QSteamInput();

    Q_INVOKABLE
    void runFrame();
};
}
