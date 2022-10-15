#pragma once

#include <QObject>

#include "iga.h"


namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
public:
    explicit QSteamInput(const QString &vdf, QObject *parent = nullptr);
    virtual ~QSteamInput();

    Q_INVOKABLE
    void runFrame();

private:

    IGA m_iga;
};
}
