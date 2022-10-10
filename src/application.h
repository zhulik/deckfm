#ifndef APPLICATION_H
#define APPLICATION_H

#include <QGuiApplication>

class QQmlApplicationEngine;

class Application : public QGuiApplication
{

public:
    explicit Application(int &argc, char **argv);

private:
    QQmlApplicationEngine *m_engine;

};

#endif // APPLICATION_H
