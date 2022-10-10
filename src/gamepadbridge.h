#pragma once

#include <QObject>

class GamepadBridge : public QObject
{
    Q_OBJECT
public:
    explicit GamepadBridge(QObject *parent = nullptr);

   Q_INVOKABLE
    void sendKeyPress(Qt::Key key);

};
