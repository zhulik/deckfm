import QtQuick 2.15
import QtMultimedia 5.15
import Qt.labs.platform 1.1

import "../QSteamworks" as Steamworks

Item {
    id: root
    property string source: `${StandardPaths.writableLocation(StandardPaths.HomeLocation)}/BigBuckBunny.mp4`

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Video {
        id: video
        anchors.fill: parent
        autoPlay: true
        source: root.source
    }

    MouseArea {
       anchors.fill: parent
       hoverEnabled: true

       property int prevX: 0

       onPositionChanged: {
            if (pressed) {
                if(prevX == 0) {
                    prevX = mouse.x
                    return
                }
                const dx = mouse.x - prevX
                video.seek(video.position + dx * 100)
                prevX = mouse.x
            } else {
                controls.show()
            }
       }
    }

    Steamworks.SteamInputScope {
        enabled: root.activeFocus

        analogHandlers: {
            "media_seek": (e) => video.seek(video.position + e.analogX * 100)
        }

        actionSet: "media_navigation"
    }

    Controls {
        id: controls
        width: 600
        height: 140

        video: video

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
    }
}
