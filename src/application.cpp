#include <QFontDatabase>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

#include "application.h"
#include "fsbridge.h"

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv)
{
    QFontDatabase::addApplicationFont(":/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");


    m_engine = new QQmlApplicationEngine();
    auto fs_bridge = new FSBridge(m_engine);
    m_engine->rootContext()->setContextProperty("fs_bridge", fs_bridge);

    m_engine->load("qrc:/qml/MainWindow.qml");

    if (m_engine->rootObjects().count() == 0) {
        qApp->exit(1);
    }
}
