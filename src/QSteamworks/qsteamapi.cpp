#include <QDir>

#include "qsteamapi.h"
#include "qsteamutils.h"
#include "qsteaminput.h"
#include "errors.h"

#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include <stdexcept>

using namespace QSteamworks;

QSteamAPI::QSteamAPI(QObject *parent)
    : QObject{parent}
{
    if (!SteamAPI_Init()) {
        throw InitializationFailed("Cannot initialize SteamAPI.");
    }
    m_steamUtils = new QSteamUtils(this);

}

QSteamAPI::~QSteamAPI()
{
    delete m_steamUtils;
    if (m_steamInput != nullptr) {
        delete m_steamInput;
    }

    SteamAPI_Shutdown();
}

void QSteamAPI::runCallbacks() const
{
    SteamAPI_RunCallbacks();
}

QSteamUtils *QSteamAPI::steamUtils() const
{
    return m_steamUtils;
}

QSteamInput *QSteamAPI::steamInput()
{
    if (m_steamInput == nullptr) {
        auto vdf = QString(qgetenv("QSTEAMWORKS_IGA_PATH"));

        if (vdf == "") {
            vdf = QDir::current().filePath("input.vdf");
        }

        m_steamInput = new QSteamInput(vdf, this);
    }
    return m_steamInput;
}
