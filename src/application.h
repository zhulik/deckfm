#pragma once

#include <QGuiApplication>

class QQmlApplicationEngine;

class Application : public QGuiApplication
{

public:
    explicit Application(int &argc, char **argv);

private:
    QQmlApplicationEngine *m_engine;

};
