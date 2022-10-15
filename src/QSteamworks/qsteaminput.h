#pragma once

#include <QObject>

#include "iga.h"


namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSteamworks::IGA iga READ iga CONSTANT)
public:
    explicit QSteamInput(const QString &vdf, QObject *parent = nullptr);
    virtual ~QSteamInput();

    Q_INVOKABLE
    void runFrame();

    const IGA &iga() const;

private:

    IGA m_iga;
};
}

