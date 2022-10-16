#include "steam/steam_api.h"

#include "steamutilsbridge.h"

SteamUtilsBridge::SteamUtilsBridge(QObject *parent) : QObject{parent} {}

bool SteamUtilsBridge::isOnDeck() const { return SteamUtils()->IsSteamRunningOnSteamDeck(); }

bool SteamUtilsBridge::isBigPicture() const { return SteamUtils()->IsSteamInBigPictureMode(); }
