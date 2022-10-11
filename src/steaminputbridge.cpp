#include <QDir>

#include "qdebug.h"

#include "steam/steam_api.h"

#include "steaminputbridge.h"

QString nameForControllerType(ESteamInputType inputType) {
    switch(inputType)
    {
    case k_ESteamInputType_SteamController:
        return "Steam";
        break;
    case k_ESteamInputType_XBox360Controller:
        return "XBox 360";
        break;
    case k_ESteamInputType_XBoxOneController:
        return "XBox One";
        break;
    case k_ESteamInputType_GenericGamepad:
        return "Generic";
        break;
    case k_ESteamInputType_PS3Controller:
        return "PS3";
        break;
    case k_ESteamInputType_PS4Controller:
        return "PS4";
        break;
    case k_ESteamInputType_AndroidController:
        return "Android";
        break;
    case k_ESteamInputType_AppleMFiController:
        return "Apple";
        break;
    case k_ESteamInputType_SteamDeckController:
        return "Steam Deck";
        break;
    case k_ESteamInputType_SwitchJoyConPair:
        return "Switch JoyCon pair";
        break;
    case k_ESteamInputType_SwitchJoyConSingle:
        return "Switch JoyCon single";
        break;
    case k_ESteamInputType_SwitchProController:
        return "Switch Pro";
        break;
    case k_ESteamInputType_MobileTouch:
        return "Mobile Touch";
        break;
    default:
        return "Unknown";
    }
}

template<typename T>
QList<T> getConnectedControllers() {
    QList<T> result;
    ControllerHandle_t handles[STEAM_INPUT_MAX_COUNT];
    auto n = SteamInput()->GetConnectedControllers( handles );

    for(int i = 0; i < n; i++) {
        result << handles[i];
    }
    return result;
}

SteamInputBridge::SteamInputBridge(QObject *parent)
    : QObject{parent}
{

}

bool SteamInputBridge::init()
{
    if (!(SteamInput()->Init(true))) {
        return false;
    }
    auto path = QDir::current().filePath("input.vdf");

    if (!SteamInput()->SetInputActionManifestFilePath( path.toLocal8Bit() )) {
        throw "error";
    }

    return true;

}

bool SteamInputBridge::shutdown()
{
    return SteamInput()->Shutdown();
}

QVariantList SteamInputBridge::GetConnectedControllers() const
{
    return getConnectedControllers<QVariant>();
}

void SteamInputBridge::poll()
{
    auto updated = getConnectedControllers<ControllerHandle_t>();

    if (updated != m_controllerHandles) {
        m_controllerHandles = updated;
        m_connectedControllers.clear();

        foreach (auto handle, updated) {
            auto inputType = SteamInput()->GetInputTypeForHandle(handle);
            auto name = nameForControllerType(inputType);

            m_connectedControllers << QVariantMap({
                                       { "handle", handle },
                                       { "type", inputType },
                                       { "name", name },
                                       { "image", QString("qrc:/resources/images/controllers/%1.png").arg(name) }
                                   });
        }
        emit connectedControllersChanged(m_connectedControllers);
    }

    if (!updated.empty()) {
        auto menuControlSet = SteamInput()->GetActionSetHandle("menu_controls");
        SteamInput()->ActivateActionSet(updated[0], menuControlSet);

        SteamAPI_RunCallbacks();

        auto menuAction = SteamInput()->GetDigitalActionHandle("menu_select");
        SteamAPI_RunCallbacks();

        auto digitalData = SteamInput()->GetDigitalActionData(updated[0], menuAction);
        auto state = digitalData.bActive;

        emit digitalActionActivated(QString("menu_select: %1").arg(state), state);
    }
}

QVariantList SteamInputBridge::connectedControllers() const
{
    return m_connectedControllers;
}
