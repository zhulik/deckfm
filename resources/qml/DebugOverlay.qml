import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "QSteamworks" as Steamworks

Item {
    id: root
    visible: false

    function toggle() {
        root.visible = !root.visible
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.6
    }

    Steamworks.SteamInputScope {
        id: input
        onInputEvent: {
            lastEventLabel.text = JSON.stringify(event, null, 2)
        }
    }

    ColumnLayout {

        anchors.fill: parent

        Label {
            text: `Action set: ${input.actionSet}`
        }

        Label {
            Layout.fillHeight: parent
            id: lastEventLabel
        }
    }
}
