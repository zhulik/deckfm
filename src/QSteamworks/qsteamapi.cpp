#include <QDir>

#include "errors.h"
#include "qsteamapi.h"
#include "qsteaminput.h"
#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include <stdexcept>

using namespace QSteamworks;

QSteamAPI::QSteamAPI(QObject *parent) : QObject{parent} {
  if (!SteamAPI_Init()) {
    throw InitializationFailed("Cannot initialize SteamAPI.");
  };
}

QSteamAPI::~QSteamAPI() { SteamAPI_Shutdown(); }

void QSteamAPI::runCallbacks() const { SteamAPI_RunCallbacks(); }
