#include <QDir>

#include "qdebug.h"

#include "steam/steam_api.h"

#include "steaminputbridge.h"

    static const QStringList actionSets{
                                        "folder_navigation",
                                        };

static const QStringList digitalGameActions{
    "folder_up",
    "folder_down",
    "folder_left",
    "folder_right",
    "folder_activate",
    "folder_go_up"
};

    static const QStringList analogGameActions{
                                               "folder_scroll",
                                               };

    static const QMap<ESteamInputType, QString> controllerNames{
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
                                                                {k_ESteamInputType_MobileTouch, "Mobile Touch"},
                                                                };

QString nameForControllerType(ESteamInputType inputType)
{
    if (controllerNames.contains(inputType)) {
        return controllerNames[inputType];
    }
    return "Unknown";
}

QList<ControllerInfo> getConnectedControllers()
{
    QList<ControllerInfo> result;
    ControllerHandle_t handles[STEAM_INPUT_MAX_COUNT];
    auto n = SteamInput()->GetConnectedControllers( handles );

    for(int i = 0; i < n; i++) {
        auto inputType = SteamInput()->GetInputTypeForHandle(handles[i]);
        auto name = nameForControllerType(inputType);

        result << ControllerInfo{
            handles[i],
            name,
            inputType,
            QString("qrc:/resources/images/controllers/%1.png").arg(name)
        };
    }
    return result;
}

SteamInputBridge::SteamInputBridge(QObject *parent)
    : QObject{parent}
{

}

void SteamInputBridge::init()
{
    if (!(SteamInput()->Init(true))) {
        throw "ERROR";
    }
    auto path = QDir::current().filePath("input.vdf");

    if (!SteamInput()->SetInputActionManifestFilePath(path.toLocal8Bit())) {
        throw "ERROR";
    }
}

void SteamInputBridge::shutdown()
{
    if (SteamInput()->Shutdown()) {
        throw "ERROR";
    }
}

void SteamInputBridge::poll()
{
    updateControllers();
    pollControllers();
}

QVariantList SteamInputBridge::connectedControllers() const
{
    QVariantList result;
    foreach(auto &info, m_connectedControllers) {
        result << QVariantMap({
            { "handle", info.handle },
            { "type", info.type },
            { "name", info.name },
            { "image", QString("qrc:/resources/images/controllers/%1.png").arg(info.name) }
        });
    }
    return result;
}

void SteamInputBridge::updateControllers()
{
    auto updated = ::getConnectedControllers();

    if(updated.empty()) {
        m_digitalActions.clear();
        emit digitalActionsChanged(digitalActions());
        m_actionSets.clear();
    }

    fillActionSets();
    fillDigitalActions();
    fillAnalogActions();

    if (updated == m_connectedControllers) {
        return;
    }

    m_connectedControllers = updated;

    emit connectedControllersChanged(connectedControllers());

    if (m_actionSet == "") {
        setActionSet("folder_navigation");
    }
}

void SteamInputBridge::pollControllers()
{
    if (m_connectedControllers.empty()) {
        return;
    }

    pollAnalogActions();
    pollDigitalActions();



    auto handle = SteamInput()->GetCurrentActionSet(m_connectedControllers[0].handle);

    QString newSet;
    foreach(auto &s, m_actionSets.toStdMap()) {
        if (s.second.handle == handle) {
            newSet = s.first;
        }
    }

    if (newSet == "") {
        throw "ERROR";
    }

    if (newSet != m_actionSet) {
        m_actionSet = newSet;
        emit actionSetChanged(newSet);
    }
}

void SteamInputBridge::pollDigitalActions()
{
    auto states = QMap<QString, bool>();

    foreach(auto &e, m_digitalActions.toStdMap()) {
        auto digitalData = SteamInput()->GetDigitalActionData(m_connectedControllers[0].handle, e.second.handle);
        states[e.first] = digitalData.bActive && digitalData.bState;
    }

    if (states != m_lastDigitalActionStates) {
        m_lastDigitalActionStates = states;
        emit digitalActionStatesChanged(digitalActionStates());
    }
}

void SteamInputBridge::pollAnalogActions()
{
    auto states = QMap<QString, QPair<float, float>>();

    foreach(auto &e, m_analogActions.toStdMap()) {
        auto analogData = SteamInput()->GetAnalogActionData(m_connectedControllers[0].handle, e.second.handle);

//        if (analogData.bActive) {
            states[e.first] = QPair(analogData.x, analogData.y);
//        }
    }

    if (states != m_lastAnalogActionStates) {
        m_lastAnalogActionStates = states;
        emit analogActionStatesChanged(analogActionStates());
    }
}

void SteamInputBridge::fillDigitalActions()
{
    if(!m_digitalActions.empty() || m_connectedControllers.empty()) {
        return;
    }

    foreach(auto &action, digitalGameActions) {
        auto handle = SteamInput()->GetDigitalActionHandle(action.toLocal8Bit());

        if (handle == 0) {
            throw "ERROR";
        }

        QList<EInputActionOrigin> origins;
        QStringList glyphs;

        EInputActionOrigin originsBuf[STEAM_INPUT_MAX_ORIGINS];

        auto n = SteamInput()->GetDigitalActionOrigins(m_connectedControllers[0].handle, m_actionSets[m_actionSet].handle, handle, originsBuf);

        for(int i = 0; i < n; i++) {
            origins << originsBuf[i];
            glyphs << SteamInput()->GetGlyphSVGForActionOrigin(originsBuf[i], 0);
        }

            m_digitalActions[action] = DigitalAction{
                                                     handle,
                                                     action,
                                                     origins,
                                                     glyphs,
                                                     };
    }
    emit digitalActionsChanged(digitalActions());
}

void SteamInputBridge::fillAnalogActions()
{
    if(!m_analogActions.empty() || m_connectedControllers.empty()) {
        return;
    }

    foreach(auto &action, analogGameActions) {
        auto handle = SteamInput()->GetAnalogActionHandle(action.toLocal8Bit());
        if (handle == 0) {
            throw "ERROR";
        }

        QList<EInputActionOrigin> origins;
        QStringList glyphs;

        EInputActionOrigin originsBuf[STEAM_INPUT_MAX_ORIGINS];

        auto n = SteamInput()->GetAnalogActionOrigins(m_connectedControllers[0].handle, m_actionSets[m_actionSet].handle, handle, originsBuf);

        for(int i = 0; i < n; i++) {
            origins << originsBuf[i];
            glyphs << SteamInput()->GetGlyphSVGForActionOrigin(originsBuf[i], 0);
        }

            m_analogActions[action] = AnalogAction{
                                                     handle,
                                                     action,
                                                     origins,
                                                     glyphs,
                                                     };
    }
    emit analogActionsChanged(analogActions());
}

void SteamInputBridge::fillActionSets()
{
    if(!m_actionSets.empty()) {
        return;
    }
    foreach(auto &set, actionSets) {
        auto handle = SteamInput()->GetActionSetHandle(set.toLocal8Bit());

        if (handle == 0) {
            throw "ERROR";
        }

            m_actionSets[set] = ActionSet{
                                          handle,
                                          set,
                                          };

    }
}

const QString &SteamInputBridge::actionSet() const
{
    return m_actionSet;
}

void SteamInputBridge::setActionSet(const QString &newActionSet)
{
    if (m_actionSet == newActionSet)
        return;
    SteamInput()->ActivateActionSet(m_connectedControllers[0].handle, m_actionSets[newActionSet].handle);
}

QVariantMap SteamInputBridge::digitalActionStates() const
{
    QVariantMap  result;
    foreach(auto &e, m_lastDigitalActionStates.toStdMap()) {
        result[e.first] = e.second;
    }
    return result;
}

QVariantMap SteamInputBridge::digitalActions() const
{
    QVariantMap  result;
    foreach(auto &e, m_digitalActions.toStdMap()) {
        result[e.first] = QVariantMap{
            {"name", e.second.name},

            {"glyphs", e.second.glyphs}
        };
    }
    return result;
}

QVariantMap SteamInputBridge::analogActions() const
{
    QVariantMap  result;
    foreach(auto &e, m_analogActions.toStdMap()) {
        result[e.first] = QVariantMap{
            {"name", e.second.name},

            {"glyphs", e.second.glyphs}
        };
    }
    return result;
}

QVariantMap SteamInputBridge::analogActionStates() const
{
    QVariantMap  result;
    foreach(auto &e, m_lastAnalogActionStates.toStdMap()) {
        result[e.first] = QVariantMap{
            {"x", e.second.first},
            {"y", e.second.second}
        };
    }
    return result;
}
