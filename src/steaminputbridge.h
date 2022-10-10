#pragma once

#include <QObject>

class SteamInputBridge : public QObject
{
    Q_OBJECT
public:
    explicit SteamInputBridge(QObject *parent = nullptr);

    Q_INVOKABLE
    bool Init(bool b);

    Q_INVOKABLE
    bool Shutdown();

signals:

};
