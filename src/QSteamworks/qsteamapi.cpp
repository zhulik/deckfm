#include "qsteamapi.h"
#include "qsteamutils.h"
#include "errors.h"

#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include <stdexcept>

using namespace QSteamworks;

QSteamAPI::QSteamAPI(QObject *parent)
    : QObject{parent}
{
    if (!SteamAPI_Init()) {
        throw InitializationFailed("Cannot initialize SteamAPI.");
    }
    m_steamUtils = new QSteamUtils(this);
}

QSteamAPI::~QSteamAPI()
{
    SteamAPI_Shutdown();
}

void QSteamAPI::runCallbacks() const
{
    SteamAPI_RunCallbacks();
}

QSteamUtils *QSteamAPI::steamUtils() const
{
    return m_steamUtils;
}
