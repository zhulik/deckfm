#pragma once

#include <QObject>
#include "steam/steam_api.h"

class SteamInputBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList connectedControllers READ connectedControllers NOTIFY connectedControllersChanged)
public:
    explicit SteamInputBridge(QObject *parent = nullptr);

    Q_INVOKABLE
    bool init();

    Q_INVOKABLE
    bool shutdown();

    Q_INVOKABLE
    QVariantList GetConnectedControllers() const;

    void poll();

    QVariantList connectedControllers() const;

signals:
    void connectedControllersChanged(QVariantList);
    void digitalActionActivated(QString, bool);

private:
    QList<ControllerHandle_t> m_controllerHandles;

    QVariantList m_connectedControllers;
};
