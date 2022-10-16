#pragma once

#include <QObject>

#include "iga.h"
#include "controller.h"


namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSteamworks::IGA iga READ iga CONSTANT)
    Q_PROPERTY(QVariantList controllers READ qmlControllers NOTIFY qmlControllersChanged) // Only connected controllers

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
    QList<Controller> m_controllers;

    void updateControllers();
};
}

