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
    QString localizedName;
    QStringList origins;
    QStringList glyphs;

    bool operator==(const DigitalAction &other) {
        return handle == other.handle;
    }
};

struct AnalogAction {
    InputAnalogActionHandle_t handle;

    QString name;
    QString localizedName;
    QStringList origins;
    QStringList glyphs;

    bool operator==(const AnalogAction &other) {
        return handle == other.handle;
    }
};


class SteamInputBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList connectedControllers READ connectedControllers NOTIFY connectedControllersChanged)

    Q_PROPERTY(QString actionSet READ actionSet WRITE setActionSet NOTIFY actionSetChanged)

    Q_PROPERTY(QVariantMap digitalActions READ digitalActions NOTIFY digitalActionsChanged)
    Q_PROPERTY(QVariantMap digitalActionStates READ digitalActionStates NOTIFY digitalActionStatesChanged)

    Q_PROPERTY(QVariantMap analogActions READ analogActions NOTIFY analogActionsChanged)
    Q_PROPERTY(QVariantMap analogActionStates READ analogActionStates NOTIFY analogActionStatesChanged)

public:
    explicit SteamInputBridge(QObject *parent = nullptr);
    virtual ~SteamInputBridge();

    Q_INVOKABLE
        void showBindingPanel();

    void poll();

    QVariantList connectedControllers() const;

    const QString &actionSet() const;
    void setActionSet(const QString &newActionSet);

    QVariantMap digitalActionStates() const;

    QVariantMap digitalActions() const;
    QVariantMap analogActions() const;

    QVariantMap analogActionStates() const;

signals:
    void connectedControllersChanged(QVariantList);
    void actionSetChanged(QString);

    void digitalActionStatesChanged(QVariantMap);
    void digitalActionsChanged(QVariantMap);
    void analogActionsChanged(QVariantMap);

    void analogActionStatesChanged(QVariantMap);

private:
    void updateControllers();
    void pollControllers();
    void pollDigitalActions();
    void pollAnalogActions();

    void fillActionSets();
    void fillDigitalActions();
    void fillAnalogActions();

    bool m_initialized = false;

    QList<ControllerInfo> m_connectedControllers;

    QMap<QString, ActionSet> m_actionSets;

    QMap<QString, DigitalAction> m_digitalActions;
    QMap<QString, AnalogAction> m_analogActions;

    QString m_actionSet;

    QMap<QString, bool> m_lastDigitalActionStates;
    QMap<QString, QPair<float, float>> m_lastAnalogActionStates;
};
