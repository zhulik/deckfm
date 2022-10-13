#include <QFontDatabase>
#include <QTimer>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickItem>

#include <QDebug>
#include <QSGRendererInterface>

#include "steam/steam_api.h"

#include "application.h"
#include "steaminputbridge.h"
#include "steamutilsbridge.h"
#include "folderlistmodel.h"



Application::Application(int &argc, char **argv)
    : QGuiApplication{argc, argv}
{
    m_steamAPIInitialized = SteamAPI_Init();

    if (!m_steamAPIInitialized) {
        qWarning() << "\n\nSteamAPI cannot be initialized.\n";
    }

    QFontDatabase::addApplicationFont(":/resources/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");

    m_engine = new QQmlApplicationEngine();

    SteamInputBridge *steamInput;

    if (m_steamAPIInitialized) {
        auto steamUtils = new SteamUtilsBridge(m_engine);
        steamInput = new SteamInputBridge(m_engine);

        if (steamUtils->isOnDeck()) {
            setOverrideCursor(QCursor(Qt::BlankCursor));
        }

        m_engine->rootContext()->setContextProperty("steam_utils", steamUtils);
        m_engine->rootContext()->setContextProperty("steam_input", steamInput);

        auto fsModel = new FolderListModel(m_engine);

        m_engine->rootContext()->setContextProperty("fs_model", fsModel);

        QObject::connect(steamInput, &SteamInputBridge::digitalActionStatesChanged, [this](auto states){
            if (m_activeFocusItem == nullptr) {
                return;
            }

            QMetaObject::invokeMethod(m_activeFocusItem, "onSteamInputDigitalStatesChanged", Q_ARG(QVariant, states));
        });
    }

    m_engine->load("qrc:/resources/qml/MainWindow.qml");

    if (m_engine->rootObjects().count() == 0) {
        throw "ERROR";
    }

    auto mainWindow = (QQuickWindow *)m_engine->rootObjects().at(0);
    QObject::connect(mainWindow, &QQuickWindow::activeFocusItemChanged, [mainWindow, this](){
        m_activeFocusItem = mainWindow->activeFocusItem();
    });

    if (m_steamAPIInitialized) {
        auto runCallbacks = [steamInput](){
            SteamAPI_RunCallbacks();
            steamInput->poll();
        };

        auto callbackTimer = new QTimer(m_engine);
        QObject::connect(callbackTimer, &QTimer::timeout, runCallbacks);
        callbackTimer->start(33);

        QObject::connect(mainWindow, &QQuickWindow::frameSwapped, runCallbacks);
    }
}

Application::~Application()
{
    if(m_steamAPIInitialized) {
        SteamAPI_Shutdown();
    }
}
