import QtQuick 2.15
import Qt.labs.platform 1.1

import "../QSteamworks" as Steamworks

Item {
    id: root
    property alias source: video.source

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Video {
        id: video
        anchors.fill: parent
    }

    Steamworks.SteamInputScope {
        enabled: root.activeFocus

        actionSet: "media_navigation"
    }

    Controls {
        id: controls
        width: parent.width
        height: 100

        deckControlsEnabled: root.activeFocus

        video: video

        anchors.bottom: parent.bottom
    }
}
