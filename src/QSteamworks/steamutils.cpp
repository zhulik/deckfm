#include "steam/steam_api.h"

#include "steamapi.h"

#include "steamutils.h"

using namespace QSteamworks;

SteamUtils::SteamUtils(QObject *parent) : QObject{parent} {}

bool SteamUtils::isSteamRunningOnSteamDeck() const { return ::SteamUtils()->IsSteamRunningOnSteamDeck(); }

bool SteamUtils::isSteamInBigPictureMode() const { return ::SteamUtils()->IsSteamInBigPictureMode(); }
