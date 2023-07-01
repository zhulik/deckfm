#include <QDir>

#include "errors.h"
#include "steamapi.h"
#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include "steaminput.h"
#include <stdexcept>

using namespace QSteamworks;

SteamAPI::SteamAPI(QObject *parent) : QObject{parent} {
  if (!SteamAPI_Init()) {
    throw InitializationFailed("Cannot initialize SteamAPI.");
  };
}

SteamAPI::~SteamAPI() { SteamAPI_Shutdown(); }

void SteamAPI::runCallbacks() const { SteamAPI_RunCallbacks(); }

bool SteamAPI::RestartAppIfNecessary() const { return SteamAPI_RestartAppIfNecessary(480); }
