#include <QFontDatabase>
#include <QTimer>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QtGamepad/QGamepadManager>

#include <QDebug>
#include <QSGRendererInterface>

#include "steam/steam_api.h"

#include "application.h"
#include "fsbridge.h"
#include "gamepadbridge.h"
#include "steaminputbridge.h"
#include "steamutilsbridge.h"



Application::Application(int &argc, char **argv)
    : QGuiApplication{argc, argv}
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QFontDatabase::addApplicationFont(":/resources/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");


    m_engine = new QQmlApplicationEngine();

    m_engine->rootContext()->setContextProperty("fs_bridge", new FSBridge(m_engine));
    m_engine->rootContext()->setContextProperty("gamepad_bridge", new GamepadBridge(m_engine));

    auto steamUtils = new SteamUtilsBridge(m_engine);

    m_engine->rootContext()->setContextProperty("steam_utils", steamUtils);
    if (steamUtils->isOnDeck()) {
        setOverrideCursor(QCursor(Qt::BlankCursor));
    }

    auto steamInput = new SteamInputBridge(m_engine);
    m_engine->rootContext()->setContextProperty("steam_input", steamInput);

    m_engine->load("qrc:/resources/qml/MainWindow.qml");

    if (m_engine->rootObjects().count() == 0) {
        throw "error";
    }
    auto mainWindow = (QQuickWindow *)m_engine->rootObjects().at(0);
    m_engine->rootContext()->setContextProperty("gamepad_bridge", new GamepadBridge(mainWindow));


    auto runCallbacks = [steamInput](){
        SteamAPI_RunCallbacks();
        steamInput->poll();
    };

    auto callbackTimer = new QTimer(m_engine);
    QObject::connect(callbackTimer, &QTimer::timeout, runCallbacks);
    callbackTimer->start(33);

    QObject::connect(mainWindow, &QQuickWindow::frameSwapped, runCallbacks);
    QObject::connect(mainWindow, &QQuickWindow::activeFocusItemChanged, [mainWindow, this](){
        m_activeFocusItem = mainWindow->activeFocusItem();
    });
}
