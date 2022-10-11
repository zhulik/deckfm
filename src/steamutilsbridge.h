#pragma once

#include <QObject>

class SteamUtilsBridge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isOnDeck READ isOnDeck NOTIFY isOnDeckChanged)
    Q_PROPERTY(bool IsSteamInBigPictureMode READ IsSteamInBigPictureMode NOTIFY IsSteamInBigPictureModeChanged)

public:
    explicit SteamUtilsBridge(QObject *parent = nullptr);


    bool isOnDeck() const;

    bool IsSteamInBigPictureMode() const;

signals:

    void isOnDeckChanged();
    void IsSteamInBigPictureModeChanged();

private:
    bool m_IsSteamRunningOnSteamDeck;
    bool m_IsSteamInBigPictureMode;
};
