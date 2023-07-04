#include "application.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  Application app(argc, argv);

  return app.exec();
}
