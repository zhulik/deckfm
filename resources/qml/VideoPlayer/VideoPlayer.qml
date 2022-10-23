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

        analogHandlers: {
            "media_seek": (e) => {
                if (e.analogX / 5) {
                    const dx = e.analogX
                    video.seek(video.position + dx * 100)
                }
            }
        }

        actionSet: "media_navigation"
    }

    Controls {
        id: controls
        width: parent.width
        height: 100

        video: video

        anchors.bottom: parent.bottom
    }
}
