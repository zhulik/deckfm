#include <QQmlApplicationEngine>

#include "application.h"

#include "steam/steam_api.h"

int main(int argc, char *argv[])
{
    if (!SteamAPI_Init()) {
        qWarning() << "SteamAPI cannot be initialized";
    }
    Application app(argc, argv);

    auto res = app.exec();

    SteamAPI_Shutdown();
    return res;
}
