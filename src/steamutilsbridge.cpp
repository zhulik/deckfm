#include "steam/steam_api.h"

#include "steamutilsbridge.h"

SteamUtilsBridge::SteamUtilsBridge(QObject *parent)
    : QObject{parent}
{

}

bool SteamUtilsBridge::IsSteamRunningOnSteamDeck() const
{
    return SteamUtils()->IsSteamRunningOnSteamDeck();
}

bool SteamUtilsBridge::IsSteamInBigPictureMode() const
{
    return SteamUtils()->IsSteamInBigPictureMode();
}
