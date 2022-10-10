#pragma once

#include <QObject>

class SteamUtilsBridge;
class SteamInputBridge;

class SteamAPIBridge : public QObject
{
    Q_OBJECT
public:
    explicit SteamAPIBridge(QObject *parent = nullptr);

    Q_INVOKABLE
    QObject *SteamUtils() const;

    Q_INVOKABLE
    QObject *SteamInput() const;

    Q_INVOKABLE
    void RunCallbacks();

private:
    SteamUtilsBridge *m_steamUtilsBridge;
    SteamInputBridge *m_steamInputBridge;
};
