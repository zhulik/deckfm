#pragma once

#include <QObject>
#include "steam/steam_api.h"

#include "QSteamworks/qsteaminput.h"


namespace QSteamworks {

class QSteamInput;

class QController : public QObject
{
    Q_OBJECT
public:
    explicit QController(QSteamInput *parent = nullptr);

private:
    ControllerHandle_t m_handle;
    QString m_name;
    ESteamInputType m_type;
    QString m_image;
};
}
