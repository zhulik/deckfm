#pragma once

#include <QObject>

class SteamUtilsBridge;

class SteamAPIBridge : public QObject
{
    Q_OBJECT
public:
    explicit SteamAPIBridge(QObject *parent = nullptr);

    Q_INVOKABLE
    QObject *SteamUtils() const;

private:
    SteamUtilsBridge *m_steamUtilsBridge;
};
