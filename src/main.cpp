#include <QQmlApplicationEngine>

#include "application.h"

#include "steam/steam_api.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  Application app(argc, argv);

  return app.exec();
}
