#include <QKeyEvent>
#include <QGuiApplication>

#include "gamepadbridge.h"

GamepadBridge::GamepadBridge(QObject *parent)
    : QObject{parent}
{

}

void GamepadBridge::sendKeyPress(Qt::Key key)
{
    auto event = new QKeyEvent(QKeyEvent::KeyPress, key, Qt::NoModifier);
    QGuiApplication::sendEvent(parent(), event);
}
