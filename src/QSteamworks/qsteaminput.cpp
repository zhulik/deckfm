#include <QFile>
#include <QGuiApplication>
#include <QMap>
#include <stdexcept>

#include "actionsetdefinition.h"
#include "actionsetlayer.h"
#include "actionsetlayerdefinition.h"
#include "controller.h"
#include "qglobal.h"
#include "qwindowdefs.h"
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

  qRegisterMetaType<InputHandle_t>("InputHandle_t");

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

void QSteamInput::onActionEvent(SteamInputActionEvent_t *event) {
  auto controller = m_controllers[event->controllerHandle];
  if (!controller) {
    return;
  }

  controller->onActionEvent(event);
}

void QSteamInput::onControllerConnected(SteamInputDeviceConnected_t *cb) {
  auto handle = cb->m_ulConnectedDeviceHandle;

  auto inputType = SteamInput()->GetInputTypeForHandle(handle);
  auto name = controllerNames.value(inputType, "Unknown");

  auto controller = new Controller(handle, name, m_iga);

  connect(this, &QSteamInput::configurationLoaded, controller, &Controller::loadActions);
  controller->moveToThread(QGuiApplication::instance()->thread());
  controller->setParent(this);

  m_controllers[handle] = controller;
  emit controllersChanged();

  qDebug() << "Controller connected:" << controller->name() << controller->handle();
}

void QSteamInput::onControllerDisconnected(SteamInputDeviceDisconnected_t *cb) {
  auto handle = cb->m_ulDisconnectedDeviceHandle;

  delete m_controllers[handle];
  m_controllers.remove(handle);

  emit controllersChanged();

  qDebug() << "Controller disconnected:" << handle;
}

void QSteamInput::onConfigurationLoaded(SteamInputConfigurationLoaded_t *) {
  auto cb = [](auto e) { QSteamInput::instance()->onActionEvent(e); };

  SteamInput()->EnableActionEventCallbacks(cb);

  emit configurationLoaded();
  qDebug() << "Confuguration loaded";
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

// void QSteamInput::setActionSetLayer(const QString &newActionSetLayer) {
//   if (m_actionSet.name() == "" || newActionSetLayer == "" || m_currentActionSetLayer.name() == newActionSetLayer) {
//     return;
//   }

//  SteamInput()->DeactivateAllActionSetLayers(m_currentController->handle());
//  m_currentActionSetLayer = ActionSetLayer();
//  emit actionSetLayerChanged();

//  foreach (auto &layer, m_actionSet.layers()) {
//    if (layer.name() == newActionSetLayer) {
//      m_currentActionSetLayer = layer;
//      SteamInput()->ActivateActionSetLayer(m_currentController->handle(), layer.handle());
//      emit actionSetLayerChanged();
//      return;
//    }
//  }
//  throw std::runtime_error(QString("Cannot find action set layer %1").arg(newActionSetLayer).toLocal8Bit());
//}

QList<Controller *> QSteamInput::controllers() const { return m_controllers.values(); }
