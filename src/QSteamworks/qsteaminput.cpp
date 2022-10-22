#include <QDebug>
#include <QFile>
#include <QMap>

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

QSteamInput::QSteamInput(const QString &vdf, QSteamAPI *parent) : QObject{parent}, m_vdf(vdf) {
  if (m_instance != nullptr) {
    throw InitializationFailed("Steam input is already initialized");
  }
  qRegisterMetaType<QSteamworks::IGA>();
  qRegisterMetaType<QSteamworks::ActionDefinition>();
  qRegisterMetaType<QSteamworks::Controller>();
  qRegisterMetaType<QSteamworks::Action>();
  qRegisterMetaType<QSteamworks::ActionSet>();
  qRegisterMetaType<QSteamworks::InputEvent>();

  if (!SteamInput()->Init(true)) {
    throw InitializationFailed("Cannot initialize SteamInput");
  }

  if (!SteamInput()->SetInputActionManifestFilePath(m_vdf.toLocal8Bit())) {
    throw InitializationFailed(QString("Cannot read IGA file: %1").arg(vdf));
  }

  auto vdfContent = readFile(vdf);
  m_iga = IGA(VDFParser().parse(vdfContent));
  SteamInput()->EnableDeviceCallbacks();

  m_instance = this;

  auto cb = [](SteamInputActionEvent_t *event) {
    QSteamInput::instance()->onActionEvent(event); // Dirty hack, but I didn't find a better way
  };

  SteamInput()->EnableActionEventCallbacks(cb);
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
  updateActionSets();
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
    updateActionSets();
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

Action QSteamInput::action(const QString &name, bool digital) const {
  foreach (auto &actionSet, m_actionSets) {
    foreach (auto &action, actionSet.actions()) {
      if (action.actionDefinition().name() == name && action.actionDefinition().isDigital() == digital) {
        return action;
      }
    }
  }
  return Action();
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
    m_actionSets << ActionSet(handle, actionSet.first, getActions(handle, actionSet.second));
  }
  SteamInput()->ActivateActionSet(m_currentController.handle(), m_actionSets.first().handle());
  emit actionSetsChanged();
}

void QSteamInput::onConfigurationLoaded(SteamInputConfigurationLoaded_t *) {
  runFrame();
  updateActionSets();
  emit configurationLoaded();
}

const QSteamworks::ActionSet &QSteamInput::actionSet() const { return m_actionSet; }

void QSteamInput::setActionSet(const QSteamworks::ActionSet &newActionSet) {
  Q_ASSERT(newActionSet.handle() != 0);

  if (m_actionSet == newActionSet)
    return;
  m_actionSet = newActionSet;
  emit actionSetChanged();
}

const QString &QSteamInput::qmlActionSet() const { return m_actionSet.name(); }

void QSteamInput::setActionSet(const QString &newActionSet) {
  if (m_actionSet.name() == newActionSet || m_currentController.handle() == 0)
    return;

  foreach (auto &actionSet, m_actionSets) {
    if (actionSet.name() == newActionSet) {
      SteamInput()->ActivateActionSet(m_currentController.handle(), actionSet.handle());
      return;
    }
  }
  throw std::runtime_error(QString("Cannot find action set %1").arg(newActionSet).toLocal8Bit());
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
