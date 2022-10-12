#pragma once

#include <QGuiApplication>

class QQmlApplicationEngine;
class QQuickItem;

class Application : public QGuiApplication
{

public:
    explicit Application(int &argc, char **argv);

private:
    QQmlApplicationEngine *m_engine;
    QQuickItem *m_activeFocusItem = 0;

};
