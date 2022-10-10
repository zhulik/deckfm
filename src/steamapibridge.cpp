#include "steamapibridge.h"
#include "steamutilsbridge.h"

SteamAPIBridge::SteamAPIBridge(QObject *parent)
    : QObject{parent}, m_steamUtilsBridge(new SteamUtilsBridge(this))
{

}

QObject *SteamAPIBridge::SteamUtils() const
{
    return m_steamUtilsBridge;
}
