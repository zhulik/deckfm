#pragma once

#include "controller.h"

#include <QObject>

#include "iga.h"

#include "steam/steam_api.h"


namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSteamworks::IGA iga READ iga CONSTANT)
    Q_PROPERTY(QVariantList controllers READ qmlControllers NOTIFY qmlControllersChanged) // Only connected controllers


    STEAM_CALLBACK( QSteamworks::QSteamInput, onControllerConnected, SteamInputDeviceConnected_t );
    STEAM_CALLBACK( QSteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t );

public:
    explicit QSteamInput(const QString &vdf, QSteamAPI *parent = nullptr);
    virtual ~QSteamInput();

    Q_INVOKABLE
    void runFrame();

    const IGA &iga() const;

    QVariantList qmlControllers() const;

signals:
    void qmlControllersChanged();

private:
    IGA m_iga;
    QSet<Controller> m_controllers;
};
}

