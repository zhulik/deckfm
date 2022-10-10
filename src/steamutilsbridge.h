#pragma once

#include <QObject>

class SteamUtilsBridge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool IsSteamRunningOnSteamDeck READ IsSteamRunningOnSteamDeck NOTIFY IsSteamRunningOnSteamDeckChanged)
public:
    explicit SteamUtilsBridge(QObject *parent = nullptr);

    bool IsSteamRunningOnSteamDeck() const;

signals:

    void IsSteamRunningOnSteamDeckChanged();
private:
    bool m_IsSteamRunningOnSteamDeck;
};
