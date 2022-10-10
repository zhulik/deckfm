#include "steam/steam_api.h"

#include "steaminputbridge.h"

SteamInputBridge::SteamInputBridge(QObject *parent)
    : QObject{parent}
{

}

bool SteamInputBridge::Init(bool b)
{
    return SteamInput()->Init(b);
}

bool SteamInputBridge::Shutdown()
{
    return SteamInput()->Shutdown();
}
