#include <QFile>
#include <QGuiApplication>
#include <QMap>
#include <stdexcept>

#include "QSteamInput/actionsetdefinition.h"
#include "QSteamInput/actionsetlayer.h"
#include "QSteamInput/actionsetlayerdefinition.h"
#include "QSteamInput/controller.h"
#include "QSteamInput/qmlsteaminputcontrol.h"
#include "qglobal.h"
#include "qwindowdefs.h"
#include "steam/isteaminput.h"
#include "steam/steam_api.h"

#include "errors.h"
#include "steamapi.h"
#include "steaminput.h"

#include "QSteamInput/vdfparser.h"
#include "steam/steamtypes.h"

using namespace QSteamworks;
using namespace QSteamworks::QSteamInput;

QSteamworks::SteamInput *QSteamworks::SteamInput::m_instance = nullptr;

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

QSteamworks::SteamInput *SteamInput::instance() { return m_instance; }

SteamInput::SteamInput(QObject *parent) : QObject{parent} {
  if (m_instance != nullptr) {
    throw InitializationFailed("Steam input is already initialized");
  }
  m_instance = this;
}

SteamInput::~SteamInput() { ::SteamInput()->Shutdown(); }

void SteamInput::runFrame() { ::SteamInput()->RunFrame(); }

IGA SteamInput::iga() const { return m_iga; }

void SteamInput::onActionEvent(SteamInputActionEvent_t *event) {
  auto controller = m_controllers[event->controllerHandle];
  if (!controller) {
    return;
  }

  controller->onActionEvent(event);
}

void SteamInput::onControllerConnected(SteamInputDeviceConnected_t *cb) {
  auto handle = cb->m_ulConnectedDeviceHandle;

  auto inputType = ::SteamInput()->GetInputTypeForHandle(handle);
  auto name = controllerNames.value(inputType, "Unknown");

  auto controller = new Controller(handle, name, m_iga);

  connect(controller, &Controller::inputEvent, controller, [controller, this](auto e) {
    if (m_lastController != controller) {
      m_lastController = controller;
      emit lastControllerChanged();
    }
    emit inputEvent(e);
  });
  connect(this, &SteamInput::configurationLoaded, controller, &Controller::loadActions);
  controller->moveToThread(QGuiApplication::instance()->thread());
  controller->setParent(this);

  m_controllers[handle] = controller;
  emit controllersChanged();

  if (m_controllers.count() == 1) { // First added controller
    m_lastController = controller;
    emit lastControllerChanged();
  }

  qDebug() << "Controller connected:" << controller->name() << controller->handle();
}

void SteamInput::onControllerDisconnected(SteamInputDeviceDisconnected_t *cb) {
  auto handle = cb->m_ulDisconnectedDeviceHandle;

  delete m_controllers[handle];
  m_controllers.remove(handle);

  emit controllersChanged();

  if (m_lastController && m_lastController->handle() == handle) {
    m_lastController = nullptr;
    emit lastControllerChanged();
  }

  qDebug() << "Controller disconnected:" << handle;
}

void SteamInput::onConfigurationLoaded(SteamInputConfigurationLoaded_t *) {
  auto cb = [](auto e) { SteamInput::instance()->onActionEvent(e); };

  ::SteamInput()->EnableActionEventCallbacks(cb);

  emit configurationLoaded();
  qDebug() << "Confuguration loaded";
}

const QString &SteamInput::igaPath() const { return m_igaPath; }

void SteamInput::setIgaPath(const QString &newIgaPath) {
  if (m_igaPath == newIgaPath)
    return;

  m_igaPath = newIgaPath;
  emit igaPathChanged();

  if (::SteamInput() == nullptr) {
    throw InitializationFailed("Cannot initialize SteamInput: SteamAPI is not initialized.");
  }

  if (!::SteamInput()->Init(true)) {
    throw InitializationFailed("Cannot initialize SteamInput: Init returned false.");
  }

  if (!::SteamInput()->SetInputActionManifestFilePath(m_igaPath.toLocal8Bit())) {
    throw InitializationFailed(QString("Cannot read IGA file: %1").arg(m_igaPath));
  }

  auto vdfContent = readFile(newIgaPath);
  m_iga = IGA(VDFParser().parse(vdfContent));
  emit igaChanged();

  ::SteamInput()->EnableDeviceCallbacks();
}

QList<Controller *> SteamInput::controllers() const { return m_controllers.values(); }

QSteamworks::QSteamInput::Controller *SteamInput::lastController() const { return m_lastController; }
