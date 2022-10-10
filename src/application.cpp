#include <QFontDatabase>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtGamepad/QGamepadManager>

#include <QDebug>

#include "application.h"
#include "fsbridge.h"
#include "gamepadbridge.h"
#include "steamapibridge.h"

Application::Application(int &argc, char **argv)
    : QGuiApplication{argc, argv}
{
    QFontDatabase::addApplicationFont(":/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");


    m_engine = new QQmlApplicationEngine();

    m_engine->rootContext()->setContextProperty("fs_bridge", new FSBridge(m_engine));
    m_engine->rootContext()->setContextProperty("gamepad_bridge", new GamepadBridge(m_engine));
    m_engine->rootContext()->setContextProperty("steam_api_bridge", new SteamAPIBridge(m_engine));

    m_engine->load("qrc:/qml/MainWindow.qml");

    if (m_engine->rootObjects().count() == 0) {
        throw "error";
    }
    m_engine->rootContext()->setContextProperty("gamepad_bridge", new GamepadBridge(m_engine->rootObjects()[0]));
}
