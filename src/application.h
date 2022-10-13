#pragma once

#include <QGuiApplication>

class QQmlApplicationEngine;
class QQuickItem;

class Application : public QGuiApplication
{

public:
    explicit Application(int &argc, char **argv);
    virtual ~Application();

private:
    QQmlApplicationEngine *m_engine;
    QQuickItem *m_activeFocusItem = nullptr;
    bool m_steamAPIInitialized = false;
};
