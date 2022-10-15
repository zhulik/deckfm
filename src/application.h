#pragma once

#include <QGuiApplication>

class QQmlApplicationEngine;
class QQuickItem;

namespace QSteamworks {
class QSteamAPI;
}

class Application : public QGuiApplication
{

public:
    explicit Application(int &argc, char **argv);
    virtual ~Application();

private:
    QQmlApplicationEngine *m_engine;
    QQuickItem *m_activeFocusItem = nullptr;
    QSteamworks::QSteamAPI *m_steamworks = nullptr;
};
