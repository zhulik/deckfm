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

#include "vdfparser.h"

#include "QSteamworks/qsteamapi.h"
#include "QSteamworks/qsteamutils.h"



Application::Application(int &argc, char **argv)
    : QGuiApplication{argc, argv}
{
    m_steamworks = new QSteamworks::QSteamAPI();

    QFontDatabase::addApplicationFont(":/resources/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");

    m_engine = new QQmlApplicationEngine();

    auto fsModel = new FolderListModel(m_engine);

    m_engine->rootContext()->setContextProperty("fs_model", fsModel);

    SteamInputBridge *steamInput;

    if (m_steamworks != nullptr) {
        steamInput = new SteamInputBridge(m_engine);

        if (m_steamworks->steamUtils()->isSteamRunningOnSteamDeck()) {
            setOverrideCursor(QCursor(Qt::BlankCursor));
        }


        m_engine->rootContext()->setContextProperty("steam_utils", m_steamworks->steamUtils());
        m_engine->rootContext()->setContextProperty("steam_input", steamInput);

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

    if (m_steamworks != nullptr) {
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
    delete m_steamworks;
}
