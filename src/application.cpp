#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>
#include <QtQuickControls2/QQuickStyle>

#include "application.h"
#include "cachednetworkaccessmanager.h"
#include "cachethumbnailimageprovider.h"
#include "diskcache.h"
#include "folderlistmodel.h"
#include "fshelpers.h"

#include "QSteamworks/errors.h"
#include "QSteamworks/steamapi.h"
#include "qnetworkaccessmanager.h"
#include "steamgamesmodel.h"

Application::Application(int &argc, char **argv) : QGuiApplication{argc, argv} {
  setOrganizationName("zhulik");
  setApplicationName("deckfm");

  QFontDatabase::addApplicationFont("resources/fonts/materialdesignicons-webfont.ttf");
  QQuickStyle::setStyle("Material");

  m_engine = new QQmlApplicationEngine();
  auto cacheThumbnailImageProvider = new CacheThumbnailImageProvider();
  cacheThumbnailImageProvider->setCache(new DiskCache(m_engine));

  qmlRegisterType<FolderListModel>("DeckFM", 1, 0, "FolderListModel");
  qmlRegisterType<SteamGamesModel>("DeckFM", 1, 0, "SteamGamesModel");

  // TODO: move to an init function in QSteamworks
  QSteamworks::registerTypes();

  qmlRegisterSingletonInstance("DeckFM", 1, 0, "FSHelpers", new FSHelpers());

  try {
    m_steamworks = new QSteamworks::SteamAPI(m_engine);
  } catch (QSteamworks::InitializationFailed &e) {
    qDebug() << "\n" << e.what() << "\n";
  }

  m_engine->rootContext()->setContextProperty("qmlEngine", m_engine);
  m_engine->rootContext()->setContextProperty("steamAPI", m_steamworks);

  m_engine->addImageProvider("cache_thumbnail", cacheThumbnailImageProvider);
  m_engine->setNetworkAccessManagerFactory(new CachedNetworkAccessManagerFactory());

  if (m_steamworks != nullptr) {
    auto callbackTimer = new QTimer(m_engine);
    connect(callbackTimer, &QTimer::timeout, m_steamworks, &QSteamworks::SteamAPI::runCallbacks);
    callbackTimer->start(16);
  }

  connect(m_engine, &QQmlApplicationEngine::objectCreated, m_engine, [this](auto obj) {
    if (obj == nullptr) {
      throw std::runtime_error("Cannot load qml.");
    }

    auto mainWindow = (QQuickWindow *)m_engine->rootObjects().at(0);

    if (arguments().count() > 1) {
      mainWindow->setProperty("openFile", arguments().at(1));
    }
  });

  m_engine->load("resources/qml/MainWindow.qml");
}

Application::~Application() { delete m_engine; }
