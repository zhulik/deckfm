#include <QFontDatabase>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlApplicationEngine>

#include <QDebug>

#include "application.h"

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv)
{
    QFontDatabase::addApplicationFont(":/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");


    m_engine = new QQmlApplicationEngine();

    m_engine->load("qrc:/qml/MainWindow.qml");

    if (m_engine->rootObjects().count() == 0) {
        qApp->exit(1);
        throw 1;
    }

}
