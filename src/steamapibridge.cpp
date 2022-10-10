#include "steam/steam_api.h"

#include "steamapibridge.h"
#include "steamutilsbridge.h"
#include "steaminputbridge.h"

SteamAPIBridge::SteamAPIBridge(QObject *parent)
    : QObject{parent}, m_steamUtilsBridge(new SteamUtilsBridge(this)),
        m_steamInputBridge(new SteamInputBridge(this))
{

}

QObject *SteamAPIBridge::SteamUtils() const
{
    return m_steamUtilsBridge;
}

QObject *SteamAPIBridge::SteamInput() const
{
    return m_steamInputBridge;
}

void SteamAPIBridge::RunCallbacks()
{
    SteamAPI_RunCallbacks();
}
