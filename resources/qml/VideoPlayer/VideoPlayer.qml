import QtQuick 2.15
import QtQuick.Controls 2.15

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

    Dialog {
        id: errorDialog
        anchors.centerIn: parent

        modal: true
        visible: video.errorString != ""
        title: `Can't play video: ${video.errorString}`
        standardButtons: Dialog.Ok

        onClosed: root.visible = false
    }


    Steamworks.SteamInputScope {
        enabled: root.activeFocus

        actionSet: "media_navigation"
    }

    Controls {
        id: controls
        anchors.fill: parent

        deckControlsEnabled: root.activeFocus

        video: video
    }
}
