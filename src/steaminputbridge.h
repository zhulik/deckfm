#pragma once

#include <QObject>
#include "steam/steam_api.h"

struct ControllerInfo {
    ControllerHandle_t handle;
    QString name;
    ESteamInputType type;
    QString image;

    bool operator==(const ControllerInfo &other) {
        return handle == other.handle;
    }
};

struct ActionSet {
    InputActionSetHandle_t handle;
    QString name;
};


struct DigitalAction {
    InputDigitalActionHandle_t handle;

    QString name;
    QList<EInputActionOrigin> origins;
    QStringList glyphs;

    bool operator==(const DigitalAction &other) {
        return handle == other.handle;
    }
};


class SteamInputBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList connectedControllers READ connectedControllers NOTIFY connectedControllersChanged)
    Q_PROPERTY(QString actionSet READ actionSet WRITE setActionSet NOTIFY actionSetChanged)
    Q_PROPERTY(QVariantMap digitalActionStates READ digitalActionStates NOTIFY digitalActionStatesChanged)
    Q_PROPERTY(QVariantMap digitalActions READ digitalActions NOTIFY digitalActionsChanged)

public:
    explicit SteamInputBridge(QObject *parent = nullptr);

    Q_INVOKABLE
        void init();

    Q_INVOKABLE
        void shutdown();

    void poll();

    QVariantList connectedControllers() const;

    const QString &actionSet() const;
    void setActionSet(const QString &newActionSet);

    QVariantMap digitalActionStates() const;

    QVariantMap digitalActions() const;

signals:
    void connectedControllersChanged(QVariantList);
    void actionSetChanged(QString);

    void digitalActionStatesChanged(QVariantMap);
    void digitalActionsChanged(QVariantMap);

private:
    void updateControllers();
    void pollControllers();
    void fillDigitalActions();
    void fillActionSets();

    QList<ControllerInfo> m_connectedControllers;

    QMap<QString, ActionSet> m_actionSets;

    QMap<QString, DigitalAction> m_digitalActions;

    QString m_actionSet;

    QMap<QString, bool> m_lastDigitalActionStates;
};
