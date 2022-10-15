#include <QMap>

#include "steam/isteaminput.h"
#include "steam/steam_api.h"

#include "qsteamapi.h"
#include "qsteaminput.h"
#include "errors.h"

using namespace QSteamworks;

static const QMap<ESteamInputType, QString> controllerNames {
    {k_ESteamInputType_SteamController, "Steam"},
    {k_ESteamInputType_XBox360Controller, "XBox 360"},
    {k_ESteamInputType_XBoxOneController, "XBox One"},
    {k_ESteamInputType_GenericGamepad, "Generic"},
    {k_ESteamInputType_PS3Controller, "PS3"},
    {k_ESteamInputType_PS4Controller, "PS4"},
    {k_ESteamInputType_PS5Controller, "PS5"},
    {k_ESteamInputType_AndroidController, "Android"},
    {k_ESteamInputType_AppleMFiController, "Apple"},
    {k_ESteamInputType_SteamDeckController, "Steam Deck"},
    {k_ESteamInputType_SwitchJoyConPair, "Switch JoyCon Pair"},
    {k_ESteamInputType_SwitchJoyConSingle, "Switch JoyCon Single"},
    {k_ESteamInputType_SwitchProController, "Switch Pro"},
    {k_ESteamInputType_MobileTouch, "Mobile Touch"}
};

static QString nameForControllerType(ESteamInputType inputType)
{
    if (controllerNames.contains(inputType)) {
        return controllerNames[inputType];
    }
    return "Unknown";
}

QSteamInput::QSteamInput(QObject *parent)
    : QObject{parent}
{
    if(!SteamInput()->Init(true)) {
        throw InitializationFailed("Cannot initialize SteamInput");
    }
}

QSteamInput::~QSteamInput()
{
    SteamInput()->Shutdown();
}

void QSteamInput::runFrame()
{
    SteamInput()->RunFrame();
}
