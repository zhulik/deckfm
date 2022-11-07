import QtQuick 2.15
import QtQuick.Controls 2.15

import "../QSteamworks" as Steamworks

Item {
    id: root

    property alias source: video.source

    signal closed()

    property var deckfmSettings: ({
                                      showCloseButton: controls.opacity === 1,
                                      showFooter: false
                                  })

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: controls.show()
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Video {
        id: video
        anchors.fill: parent
    }

    Dialog {
        id: errorDialog
        anchors.centerIn: parent

        modal: true
        visible: video.errorString != ""
        title: `Can't play video: ${video.errorString}`
        standardButtons: Dialog.Ok

        onClosed: root.closed()
    }

    Steamworks.SteamInputScope {
        enabled: root.activeFocus

        actionSetLayer: "media_controls"
    }

    Controls {
        id: controls
        anchors.fill: parent

        deckControlsEnabled: root.activeFocus

        video: video
    }
}
