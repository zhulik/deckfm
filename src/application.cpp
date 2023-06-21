#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>
#include <QtQuickControls2/QQuickStyle>

#include <QDebug>

#include "application.h"
#include "folderlistmodel.h"
#include "fshelpers.h"

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
  qmlRegisterType<QSteamworks::QSteamInput>("Steamworks", 1, 0, "SteamInput");
  qmlRegisterSingletonInstance("DeckFM", 1, 0, "FSHelpers", new FSHelpers());

  m_engine = new QQmlApplicationEngine();

  try {
    m_steamworks = new QSteamworks::QSteamAPI(m_engine);
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
      auto runCallbacks = [this]() { m_steamworks->runCallbacks(); };

      auto callbackTimer = new QTimer(m_engine);
      QObject::connect(callbackTimer, &QTimer::timeout, runCallbacks);
      callbackTimer->start(33);

      QObject::connect(mainWindow, &QQuickWindow::frameSwapped, runCallbacks);
    }

    if (arguments().count() > 1) {
      mainWindow->setProperty("openFile", arguments().at(1));
    }
  });

  m_engine->load("resources/qml/MainWindow.qml");
}

Application::~Application() { delete m_engine; }
