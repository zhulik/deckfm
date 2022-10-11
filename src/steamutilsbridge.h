#pragma once

#include <QObject>

class SteamUtilsBridge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool IsSteamRunningOnSteamDeck READ IsSteamRunningOnSteamDeck NOTIFY IsSteamRunningOnSteamDeckChanged)
    Q_PROPERTY(bool IsSteamInBigPictureMode READ IsSteamInBigPictureMode NOTIFY IsSteamInBigPictureModeChanged)

public:
    explicit SteamUtilsBridge(QObject *parent = nullptr);

    bool IsSteamRunningOnSteamDeck() const;

    bool IsSteamInBigPictureMode() const;

signals:

    void IsSteamRunningOnSteamDeckChanged();
    void IsSteamInBigPictureModeChanged();

private:
    bool m_IsSteamRunningOnSteamDeck;
    bool m_IsSteamInBigPictureMode;
};
