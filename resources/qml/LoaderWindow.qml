import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    function load(path) {
        loader.source = `file://${path}`
        visible = true
    }

    visibility: {
        if (visible) {
            steam_utils.isSteamRunningOnSteamDeck
                    || steam_utils.isSteamInBigPictureMode ? "FullScreen" : "Windowed"
        } else {
            "Hidden"
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
    }
}
