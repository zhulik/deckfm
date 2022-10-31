#include <QDebug>
#include <QFile>
#include <QMap>

#include "actionsetdefinition.h"
#include "actionsetlayer.h"
#include "actionsetlayerdefinition.h"
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

static QString nameForControllerType(ESteamInputType inputType) {
  if (controllerNames.contains(inputType)) {
    return controllerNames[inputType];
  }
  return "Unknown";
}

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
  qRegisterMetaType<QSteamworks::ActionSetDefinition>();
  qRegisterMetaType<QSteamworks::ActionSetLayerDefinition>();
  qRegisterMetaType<QSteamworks::Controller>();
  qRegisterMetaType<QSteamworks::Action>();
  qRegisterMetaType<QSteamworks::ActionSet>();
  qRegisterMetaType<QSteamworks::ActionSetLayer>();
  qRegisterMetaType<QSteamworks::InputEvent>();

  m_instance = this;
}

QSteamInput::~QSteamInput() { SteamInput()->Shutdown(); }

void QSteamInput::runFrame() {
  SteamInput()->RunFrame();

  if (m_currentController.handle() != 0 && !m_actionSets.empty()) {
    auto handle = SteamInput()->GetCurrentActionSet(m_currentController.handle());

    foreach (auto &actionSet, m_actionSets) {
      if (actionSet.handle() == handle) {
        setActionSet(actionSet);
      }
    }
  }
}

bool QSteamInput::showBindingPanel(unsigned long long inputHandle) const {
  return SteamInput()->ShowBindingPanel(inputHandle);
}

bool QSteamInput::showBindingPanel() const {
  if (m_currentController.handle() == 0) {
    return false;
  }
  return SteamInput()->ShowBindingPanel(m_currentController.handle());
}

void QSteamInput::triggerSimpleHapticEvent(const QString &location, unsigned char nIntensity, char nGainDB,
                                           unsigned char nOtherIntensity, char nOtherGainDB) const {
  if (m_currentController.handle() == 0) {
    return;
  }

  auto eLocation = k_EControllerHapticLocation_Left;
  if (location == "left") {
    eLocation = k_EControllerHapticLocation_Left;
  } else if (location == "right") {
    eLocation = k_EControllerHapticLocation_Right;
  } else {
    eLocation = k_EControllerHapticLocation_Both;
  }
  SteamInput()->TriggerSimpleHapticEvent(m_currentController.handle(), eLocation, nIntensity, nGainDB, nOtherIntensity,
                                         nOtherGainDB);
}

const IGA &QSteamInput::iga() const { return m_iga; }

QVariantList QSteamInput::qmlControllers() const {
  QVariantList result;

  foreach (auto &controller, m_controllers) {
    result << QVariant::fromValue(controller);
  }

  return result;
}

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
  foreach (auto &controller, m_controllers) {
    if (controller.handle() == event->controllerHandle) {
      setCurrentController(controller);
    }
  }

  unsigned long long actionHandle = 0;
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

  auto a = action(actionHandle, event->eEventType == ESteamInputActionEventType_DigitalAction);

  Q_ASSERT(a.handle() != 0);

  updateActionStates(a, digitalState, analogX, analogY);

  auto iEvent = InputEvent(type, m_currentController, a, digitalState, analogX, analogY);

  sendInputEvents(iEvent);
}

void QSteamInput::onControllerConnected(SteamInputDeviceConnected_t *cb) {
  auto handle = cb->m_ulConnectedDeviceHandle;

  auto inputType = SteamInput()->GetInputTypeForHandle(handle);
  auto name = nameForControllerType(inputType);
  auto image = QString("resources/images/controllers/%1.png").arg(name);

  auto controller = Controller(handle, name, image);
  m_controllers << controller;
  setCurrentController(controller);
  emit qmlControllersChanged();

  runFrame();
}

void QSteamInput::onControllerDisconnected(SteamInputDeviceDisconnected_t *cb) {
  auto handle = cb->m_ulDisconnectedDeviceHandle;
  auto controller = Controller(handle, "", "");
  if (m_controllers.contains(controller)) {
    m_controllers.remove(controller);
  }

  emit qmlControllersChanged();

  if (!m_controllers.empty()) {
    m_currentController = Controller();
  }
}

const Controller &QSteamInput::currentController() const { return m_currentController; }

QVariantList QSteamInput::qmlActionSets() const {
  QVariantList result;

  foreach (auto &actionSet, m_actionSets) {
    result << QVariant::fromValue(actionSet);
  }

  return result;
}

void QSteamInput::setCurrentController(const Controller &newCurrentController) {
  if (m_currentController == newCurrentController)
    return;

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

    unsigned long long handle = 0;
    int n = 0;
    QString localizedName;

    if (action.isDigital()) {
      handle = SteamInput()->GetDigitalActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = SteamInput()->GetDigitalActionOrigins(m_currentController.handle(), actionSetHandle, handle,
                                                originsBuf.data());

      localizedName = SteamInput()->GetStringForDigitalActionName(handle);
    } else {
      handle = SteamInput()->GetAnalogActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = SteamInput()->GetAnalogActionOrigins(m_currentController.handle(), actionSetHandle, handle,
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

Action QSteamInput::action(unsigned long long handle, bool digital) const {
  foreach (auto &actionSet, m_actionSets) {
    foreach (auto &action, actionSet.actions()) {
      if (action.handle() == handle && action.actionDefinition().isDigital() == digital) {
        return action;
      }
    }
  }
  return Action();
}

void QSteamInput::updateActionSets() {
  m_actionSets.clear();

  foreach (auto &actionSet, m_iga.actionSets().toStdMap()) {
    auto handle = SteamInput()->GetActionSetHandle(actionSet.first.toLocal8Bit());
    m_actionSets << ActionSet(handle, actionSet.first, getActions(handle, actionSet.second.actions()),
                              getActionSetLayers(actionSet.second.layers()));
  }
  emit actionSetsChanged();
}

void QSteamInput::onConfigurationLoaded(SteamInputConfigurationLoaded_t *) {
  runFrame();
  updateActionSets();
  setActionSet(m_defaultActionSet);
  emit configurationLoaded();
}

const QSteamworks::ActionSet &QSteamInput::actionSet() const { return m_actionSet; }

void QSteamInput::setActionSet(const QSteamworks::ActionSet &newActionSet) {
  Q_ASSERT(newActionSet.handle() != 0);

  if (m_actionSet == newActionSet)
    return;

  m_actionSet = newActionSet;

  if (m_currentController.handle() != 0) {
    SteamInput()->ActivateActionSet(m_currentController.handle(), m_actionSet.handle());
  }

  emit actionSetChanged();
}

const QString &QSteamInput::qmlActionSet() const { return m_actionSet.name(); }

void QSteamInput::setActionSet(const QString &newActionSet) {
  if (m_actionSet.name() == newActionSet || newActionSet == "")
    return;

  if (!m_iga.actionSets().contains(newActionSet)) {
    throw std::runtime_error(QString("Cannot find action set %1").arg(newActionSet).toLocal8Bit());
  }

  foreach (auto &actionSet, m_actionSets) {
    if (actionSet.name() == newActionSet) {
      setActionSet(actionSet);
      return;
    }
  }
}

unsigned short QSteamInput::vibrationSpeedLeft() const { return m_vibrationSpeedLeft; }

void QSteamInput::setVibrationSpeedLeft(unsigned short newVibrationSpeedLeft) {
  if (m_vibrationSpeedLeft == newVibrationSpeedLeft)
    return;
  m_vibrationSpeedLeft = newVibrationSpeedLeft;
  if (m_currentController.handle() != 0) {
    SteamInput()->TriggerVibration(m_currentController.handle(), m_vibrationSpeedLeft, m_vibrationSpeedRight);
  }
  emit vibrationSpeedLeftChanged();
}

unsigned short QSteamInput::vibrationSpeedRight() const { return m_vibrationSpeedRight; }

void QSteamInput::setVibrationSpeedRight(unsigned short newVibrationSpeedRight) {
  if (m_vibrationSpeedRight == newVibrationSpeedRight)
    return;
  m_vibrationSpeedRight = newVibrationSpeedRight;
  if (m_currentController.handle() != 0) {
    SteamInput()->TriggerVibration(m_currentController.handle(), m_vibrationSpeedLeft, m_vibrationSpeedRight);
  }
  emit vibrationSpeedRightChanged();
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

  auto cb = [](SteamInputActionEvent_t *event) {
    QSteamInput::instance()->onActionEvent(event); // Dirty hack, but I didn't find a better way
  };

  SteamInput()->EnableActionEventCallbacks(cb);
}

const QString &QSteamInput::defaultActionSet() const { return m_defaultActionSet; }

void QSteamInput::setDefaultActionSet(const QString &newDefaultActionSet) {
  if (m_defaultActionSet == newDefaultActionSet)
    return;
  m_defaultActionSet = newDefaultActionSet;
  emit defaultActionSetChanged();
}

const QSteamworks::ActionSet &QSteamInput::currentActionSet() const { return m_actionSet; }

const QSteamworks::ActionSetLayer &QSteamInput::currentActionSetLayer() const { return m_currentActionSetLayer; }

const QString &QSteamInput::qmlActionSetLayer() const { return m_currentActionSetLayer.name(); }

void QSteamInput::setActionSetLayer(const QString &newActionSetLayer) {
  if (m_actionSet.name() == "" || newActionSetLayer == "" || m_currentActionSetLayer.name() == newActionSetLayer)
    return;

  if (!m_iga.qmlActionSetLayers().contains(newActionSetLayer)) {
    throw std::runtime_error(QString("Cannot find action set layer %1").arg(newActionSetLayer).toLocal8Bit());
  }

  SteamInput()->DeactivateAllActionSetLayers(m_currentController.handle());

  foreach (auto &layer, m_actionSet.layers()) {
    if (layer.name() == newActionSetLayer) {
      m_currentActionSetLayer = layer;
      SteamInput()->ActivateActionSetLayer(m_currentController.handle(), layer.handle());
      emit actionSetLayerChanged();
      return;
    }
  }
  //  m_currentActionSetLayer = ActionSetLayer(-1, m_actionSet.name(), QList<Action>());
  //  //  emit actionSetLayerChanged();
  throw std::runtime_error(QString("Cannot find action set layer %1").arg(newActionSetLayer).toLocal8Bit());
}
