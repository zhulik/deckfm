#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>
#include <QtQuickControls2/QQuickStyle>

#include <QDebug>

#include "steam/steam_api.h"

#include "application.h"
#include "folderlistmodel.h"

#include "QSteamworks/errors.h"
#include "QSteamworks/qsteamapi.h"
#include "QSteamworks/qsteaminput.h"
#include "QSteamworks/qsteamutils.h"

Application::Application(int &argc, char **argv) : QGuiApplication{argc, argv} {
  setOrganizationName("zhulik");
  setApplicationName("deckfm");

  QFontDatabase::addApplicationFont("resources/fonts/materialdesignicons-webfont.ttf");
  QQuickStyle::setStyle("Material");

  qmlRegisterType<FolderListModel>("DeckFM", 1, 0, "FolderListModel");

  qmlRegisterType<QSteamworks::QSteamUtils>("Steamworks", 1, 0, "SteamUtils");

  m_engine = new QQmlApplicationEngine();

  try {
    m_steamworks = new QSteamworks::QSteamAPI(m_engine);

    m_engine->rootContext()->setContextProperty("steam_input", m_steamworks->steamInput());
  } catch (QSteamworks::InitializationFailed &e) {
    qDebug() << "\n" << e.what() << "\n";
  }

  connect(m_engine, &QQmlApplicationEngine::objectCreated, [this](auto obj) {
    if (obj == nullptr) {
      throw std::runtime_error("Cannot load qml.");
    }

    auto mainWindow = (QQuickWindow *)m_engine->rootObjects().at(0);
    QObject::connect(mainWindow, &QQuickWindow::activeFocusItemChanged,
                     [mainWindow, this]() { m_activeFocusItem = mainWindow->activeFocusItem(); });

    if (m_steamworks != nullptr) {
      auto runCallbacks = [this]() {
        m_steamworks->runCallbacks();
        m_steamworks->steamInput()->runFrame();
      };

      auto callbackTimer = new QTimer(m_engine);
      QObject::connect(callbackTimer, &QTimer::timeout, runCallbacks);
      callbackTimer->start(33);

      QObject::connect(mainWindow, &QQuickWindow::frameSwapped, runCallbacks);
    }
  });

  QString qmlPath = "resources/qml/MainWindow.qml";

  if (arguments().count() > 1) {
    qmlPath = arguments().at(1);
  }

  m_engine->load(qmlPath);
}

Application::~Application() { delete m_engine; }
