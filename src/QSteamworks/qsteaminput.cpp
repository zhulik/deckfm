#include <QFile>
#include <QMap>

#include "actionsetdefinition.h"
#include "actionsetlayer.h"
#include "actionsetlayerdefinition.h"
#include "qglobal.h"
#include "steam/isteaminput.h"
#include "steam/steam_api.h"

#include "errors.h"
#include "qsteamapi.h"
#include "qsteaminput.h"

#include "vdfparser.h"

using namespace QSteamworks;

QSteamworks::QSteamInput *QSteamworks::QSteamInput::m_instance = nullptr;

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
    {k_ESteamInputType_MobileTouch, "Mobile Touch"}};

QString readFile(QString const &path) {
  QFile f(path);

  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    throw std::runtime_error(QString("Cannot read file %1").arg(path).toLocal8Bit());
  }

  return QTextStream(&f).readAll();
}

QSteamInput *QSteamInput::instance() { return m_instance; }

QSteamInput::QSteamInput(QObject *parent) : QObject{parent} {
  if (m_instance != nullptr) {
    throw InitializationFailed("Steam input is already initialized");
  }
  qRegisterMetaType<QSteamworks::IGA>();

  qRegisterMetaType<QSteamworks::ActionDefinition>();
  qRegisterMetaType<QList<QSteamworks::ActionDefinition>>();

  qRegisterMetaType<QSteamworks::ActionSetDefinition>();
  qRegisterMetaType<QList<QSteamworks::ActionSetDefinition>>();

  qRegisterMetaType<QSteamworks::ActionSetLayerDefinition>();
  qRegisterMetaType<QList<QSteamworks::ActionSetLayerDefinition>>();

  qRegisterMetaType<QSteamworks::Action>();
  qRegisterMetaType<QSteamworks::ActionSet>();
  qRegisterMetaType<QSteamworks::ActionSetLayer>();
  qRegisterMetaType<QSteamworks::InputEvent>();

  m_instance = this;
}

QSteamInput::~QSteamInput() { SteamInput()->Shutdown(); }

void QSteamInput::runFrame() { SteamInput()->RunFrame(); }

IGA QSteamInput::iga() const { return m_iga; }

void QSteamInput::updateActionStates(const Action &action, bool digitalState, float analogX, float analogY) {
  QVariant state;

  if (action.actionDefinition().isDigital()) {
    state = digitalState;
  } else {
    state = QVariantMap{{"x", analogX}, {"y", analogY}};
  }

  m_actionStates[action.actionDefinition().name()] = state;

  emit actionStatesChanged();
}

void QSteamworks::QSteamInput::sendInputEvents(InputEvent e) {
  emit inputEvent(e);

  if (!e.action().actionDefinition().isDigital()) {
    emit analogEvent(e);
    return;
  }

  if (e.digitalState()) {
    emit pressedEvent(e);
  } else {
    emit releasedEvent(e);
  }
}

QList<ActionSetLayer> QSteamInput::getActionSetLayers(const QList<ActionSetLayerDefinition> &definitions) const {
  QList<ActionSetLayer> result;
  foreach (auto &definition, definitions) {

    auto handle = SteamInput()->GetActionSetHandle(definition.name().toLocal8Bit());
    result << ActionSetLayer(handle, definition.name(), getActions(handle, definition.actions()));
  }
  return result;
}

void QSteamInput::onActionEvent(SteamInputActionEvent_t *event) {
  setCurrentController(m_controllers[event->controllerHandle]);

  InputHandle_t actionHandle = 0;
  QString type;
  bool digitalState = false;
  float analogX = 0;
  float analogY = 0;

  if (event->eEventType == ESteamInputActionEventType_DigitalAction) {
    type = "digital";
    actionHandle = event->digitalAction.actionHandle;

    digitalState = event->digitalAction.digitalActionData.bState;
  } else {
    type = "analog";
    actionHandle = event->analogAction.actionHandle;

    // TODO: add support for event->analogAction.analogActionData.eMode
    analogX = event->analogAction.analogActionData.x;
    analogY = event->analogAction.analogActionData.y;
  }

  auto a = actionByHandle(actionHandle, event->eEventType == ESteamInputActionEventType_DigitalAction);

  //  Q_ASSERT(a.handle() != 0);

  updateActionStates(a, digitalState, analogX, analogY);

  auto iEvent = InputEvent(type, m_controllers[event->controllerHandle], a, digitalState, analogX, analogY);

  sendInputEvents(iEvent);
}

const Action QSteamInput::actionByName(const QString &name) const {
  foreach (auto &action, m_actionSet.actions()) {
    if (action.actionDefinition().name() == name) {
      return action;
    }
  }

  foreach (auto &layer, m_actionSet.layers()) {
    foreach (auto &action, layer.actions()) {
      if (action.actionDefinition().name() == name) {
        return action;
      }
    }
  }

  return Action();
}

void QSteamInput::onControllerConnected(SteamInputDeviceConnected_t *cb) {
  auto handle = cb->m_ulConnectedDeviceHandle;

  auto inputType = SteamInput()->GetInputTypeForHandle(handle);
  auto name = controllerNames.value(inputType, "Unknown");

  auto controller = new Controller(handle, name, this);
  m_controllers[handle] = controller;
  emit controllersChanged();

  setCurrentController(controller);
}

void QSteamInput::onControllerDisconnected(SteamInputDeviceDisconnected_t *cb) {
  auto handle = cb->m_ulDisconnectedDeviceHandle;

  delete m_controllers[handle];
  m_controllers.remove(handle);

  emit controllersChanged();

  if (m_currentController->handle() != handle) {
    return;
  }

  if (!m_controllers.empty()) {
    setCurrentController(nullptr);
  } else {
    setCurrentController(m_controllers[0]);
  }
}

Controller *QSteamInput::currentController() const { return m_currentController; }

QVariantList QSteamInput::qmlActionSets() const {
  QVariantList result;

  foreach (auto &actionSet, m_actionSets) {
    result << QVariant::fromValue(actionSet);
  }

  return result;
}

void QSteamInput::setCurrentController(Controller *newCurrentController) {
  if (m_currentController == newCurrentController) {
    return;
  }

  m_currentController = newCurrentController;
  emit currentControllerChanged();
}

QList<Action> QSteamInput::getActions(InputActionSetHandle_t actionSetHandle,
                                      const QList<ActionDefinition> &actions) const {
  QList<Action> result;

  foreach (auto &action, actions) {
    QStringList origins;
    QStringList glyphs;

    QVector<EInputActionOrigin> originsBuf(STEAM_INPUT_MAX_ORIGINS);

    InputHandle_t handle = 0;
    int n = 0;
    QString localizedName;

    if (action.isDigital()) {
      handle = SteamInput()->GetDigitalActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = SteamInput()->GetDigitalActionOrigins(m_currentController->handle(), actionSetHandle, handle,
                                                originsBuf.data());

      localizedName = SteamInput()->GetStringForDigitalActionName(handle);
    } else {
      handle = SteamInput()->GetAnalogActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = SteamInput()->GetAnalogActionOrigins(m_currentController->handle(), actionSetHandle, handle,
                                               originsBuf.data());
      localizedName = SteamInput()->GetStringForAnalogActionName(handle);
    }
    originsBuf.resize(n);

    foreach (auto &origin, originsBuf) {
      origins << SteamInput()->GetStringForActionOrigin(origin);
      glyphs << SteamInput()->GetGlyphSVGForActionOrigin(origin, 0);
    }

    result << Action(handle, action, localizedName, origins, glyphs);
  }
  return result;
}

const Action &QSteamInput::actionByHandle(InputHandle_t handle, bool digital) const {
  foreach (auto &actionSet, m_actionSets) {
    foreach (auto &action, actionSet.actions()) {
      if (action.handle() == handle && action.actionDefinition().isDigital() == digital) {
        return action;
      }
    }
    foreach (auto &layer, actionSet.layers()) {
      foreach (auto &action, layer.actions()) {
        if (action.handle() == handle && action.actionDefinition().isDigital() == digital) {
          return action;
        }
      }
    }
  }
  return Action();
}

void QSteamInput::loadActionSets() {
  m_actionSets.clear();

  foreach (auto &actionSet, m_iga.actionSets()) {
    auto handle = SteamInput()->GetActionSetHandle(actionSet.name().toLocal8Bit());
    m_actionSets << ActionSet(handle, actionSet.name(), getActions(handle, actionSet.actions()),
                              getActionSetLayers(actionSet.layers()));
  }
  emit actionSetsChanged();
}

void QSteamInput::onConfigurationLoaded(SteamInputConfigurationLoaded_t *) {
  loadActionSets();

  // Dirty hack, but I didn't find a better way
  auto cb = [](auto e) { QSteamInput::instance()->onActionEvent(e); };

  SteamInput()->EnableActionEventCallbacks(cb);

  emit configurationLoaded();
}

const QSteamworks::ActionSet &QSteamInput::actionSet() const { return m_actionSet; }

void QSteamInput::setActionSet(const QSteamworks::ActionSet &newActionSet) {
  Q_ASSERT(newActionSet.handle() != 0);

  if (m_actionSet == newActionSet)
    return;

  m_actionSet = newActionSet;

  if (m_currentController) {
    SteamInput()->ActivateActionSet(m_currentController->handle(), m_actionSet.handle());
  }

  emit actionSetChanged();
}

const QString &QSteamInput::qmlActionSet() const { return m_actionSet.name(); }

void QSteamInput::setActionSet(const QString &newActionSet) {
  if (m_actionSet.name() == newActionSet || newActionSet == "")
    return;

  foreach (auto &actionSet, m_actionSets) {
    if (actionSet.name() == newActionSet) {
      setActionSet(actionSet);
      return;
    }
  }
  //  throw std::runtime_error(QString("Cannot find action set %1").arg(newActionSet).toLocal8Bit());
}

const QVariantMap &QSteamInput::actionStates() const { return m_actionStates; }

const QString &QSteamInput::igaPath() const { return m_igaPath; }

void QSteamInput::setIgaPath(const QString &newIgaPath) {
  if (m_igaPath == newIgaPath)
    return;

  m_igaPath = newIgaPath;
  emit igaPathChanged();

  if (SteamInput() == nullptr) {
    throw InitializationFailed("Cannot initialize SteamInput: SteamAPI is not initialized.");
  }

  if (!SteamInput()->Init(true)) {
    throw InitializationFailed("Cannot initialize SteamInput: Init returned false.");
  }

  if (!SteamInput()->SetInputActionManifestFilePath(m_igaPath.toLocal8Bit())) {
    throw InitializationFailed(QString("Cannot read IGA file: %1").arg(m_igaPath));
  }

  auto vdfContent = readFile(newIgaPath);
  m_iga = IGA(VDFParser().parse(vdfContent));
  emit igaChanged();

  SteamInput()->EnableDeviceCallbacks();
}

const QSteamworks::ActionSet &QSteamInput::currentActionSet() const { return m_actionSet; }

const QSteamworks::ActionSetLayer &QSteamInput::currentActionSetLayer() const { return m_currentActionSetLayer; }

const QString &QSteamInput::qmlActionSetLayer() const { return m_currentActionSetLayer.name(); }

void QSteamInput::setActionSetLayer(const QString &newActionSetLayer) {
  if (m_actionSet.name() == "" || newActionSetLayer == "" || m_currentActionSetLayer.name() == newActionSetLayer) {
    return;
  }

  SteamInput()->DeactivateAllActionSetLayers(m_currentController->handle());
  m_currentActionSetLayer = ActionSetLayer();
  emit actionSetLayerChanged();

  foreach (auto &layer, m_actionSet.layers()) {
    if (layer.name() == newActionSetLayer) {
      m_currentActionSetLayer = layer;
      SteamInput()->ActivateActionSetLayer(m_currentController->handle(), layer.handle());
      emit actionSetLayerChanged();
      return;
    }
  }
  throw std::runtime_error(QString("Cannot find action set layer %1").arg(newActionSetLayer).toLocal8Bit());
}

QMap<InputHandle_t, Controller *> QSteamInput::controllers() const { return m_controllers; }
