#include <QQmlApplicationEngine>

#include "application.h"

#include "steam/steam_api.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    if (!SteamAPI_Init()) {
        qWarning() << "SteamAPI cannot be initialized";
    }
    Application app(argc, argv);

    return app.exec();
}
