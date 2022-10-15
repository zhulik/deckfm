#pragma once

#include <QObject>

namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject
{
    Q_OBJECT
public:
    explicit QSteamInput(QSteamAPI *parent = nullptr);

signals:

};
}
