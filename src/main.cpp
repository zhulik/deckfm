#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>

#include "application.h"

#include "steam/steam_api.h"

int main(int argc, char *argv[]) {
  //  QtWebEngine::initialize();

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  Application app(argc, argv);

  return app.exec();
}
