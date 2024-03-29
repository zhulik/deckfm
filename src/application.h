#pragma once

#include <QGuiApplication>

class QQmlApplicationEngine;
class QQuickItem;

namespace QSteamworks {
class SteamAPI;
}

class CacheThumbnailImageProvider;

class Application : public QGuiApplication {

public:
  explicit Application(int &argc, char **argv);
  virtual ~Application() override;

private:
  QQmlApplicationEngine *m_engine = nullptr;
  QSteamworks::SteamAPI *m_steamworks = nullptr;
};
