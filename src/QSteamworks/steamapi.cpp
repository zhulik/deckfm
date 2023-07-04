#include <QDir>
#include <QQmlEngine>

#include "errors.h"
#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include "steamapi.h"
#include "steaminput.h"
#include <stdexcept>

#include "QSteamworks/QSteamInput/actionset.h"
#include "QSteamworks/QSteamInput/controller.h"
#include "QSteamworks/QSteamInput/qmlsteaminputcontrol.h"
#include "QSteamworks/QSteamInput/qmlsteaminputscope.h"
#include "QSteamworks/steamutils.h"

using namespace QSteamworks;

SteamAPI::SteamAPI(QObject *parent) : QObject{parent} {
  if (!SteamAPI_Init()) {
    throw InitializationFailed("Cannot initialize SteamAPI.");
  };
}

SteamAPI::~SteamAPI() { SteamAPI_Shutdown(); }

void SteamAPI::runCallbacks() const { SteamAPI_RunCallbacks(); }

bool SteamAPI::RestartAppIfNecessary() const { return SteamAPI_RestartAppIfNecessary(480); }

void QSteamworks::registerTypes() {
  qmlRegisterType<QSteamworks::SteamUtils>("Steamworks", 1, 0, "SteamUtils");
  qmlRegisterType<QSteamworks::SteamInput>("Steamworks", 1, 0, "SteamInput");
  qmlRegisterType<QSteamworks::QSteamInput::QMLSteamInputControl>("Steamworks.SteamInput", 1, 0, "SteamInputControl");
  qmlRegisterType<QSteamworks::QSteamInput::QMLSteamInputScope>("Steamworks.SteamInput", 1, 0, "SteamInputScope");

  qRegisterMetaType<QSteamworks::QSteamInput::IGA>();

  qRegisterMetaType<QSteamworks::QSteamInput::ActionDefinition>();
  qRegisterMetaType<QList<QSteamworks::QSteamInput::ActionDefinition>>();

  qRegisterMetaType<QSteamworks::QSteamInput::ActionSetDefinition>();
  qRegisterMetaType<QList<QSteamworks::QSteamInput::ActionSetDefinition>>();

  qRegisterMetaType<QSteamworks::QSteamInput::ActionSetLayerDefinition>();
  qRegisterMetaType<QList<QSteamworks::QSteamInput::ActionSetLayerDefinition>>();

  qRegisterMetaType<QSteamworks::QSteamInput::Action>();
  qRegisterMetaType<QSteamworks::QSteamInput::ActionSet>();
  qRegisterMetaType<QSteamworks::QSteamInput::ActionSetLayer>();
  qRegisterMetaType<QSteamworks::QSteamInput::InputEvent>();

  qRegisterMetaType<InputHandle_t>("InputHandle_t");
  qRegisterMetaType<uint8>("uint8");
  qRegisterMetaType<int8>("int8");

  qRegisterMetaType<QSteamworks::QSteamInput::Controller *>();
  qRegisterMetaType<QList<QSteamworks::QSteamInput::Controller *>>();
}
