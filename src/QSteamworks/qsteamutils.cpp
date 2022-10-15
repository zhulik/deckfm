#include "steam/steam_api.h"

#include "qsteamapi.h"

#include "qsteamutils.h"


using namespace QSteamworks;

QSteamUtils::QSteamUtils(QObject *parent)
    : QObject{parent}
{

}

bool QSteamUtils::isSteamRunningOnSteamDeck() const
{
    return SteamUtils()->IsSteamRunningOnSteamDeck();
}

bool QSteamUtils::isSteamInBigPictureMode() const
{
    return SteamUtils()->IsSteamInBigPictureMode();
}
