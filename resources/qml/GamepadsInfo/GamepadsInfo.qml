import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../models" as Models

ColumnLayout {
    anchors.fill: parent

    TabBar {
        id: tabBar
        Layout.preferredWidth: parent.width
        Layout.alignment: Qt.AlignTop

        currentIndex: view.currentIndex

        Repeater {
            id: tabRepeater
            //                model: [
            //                    {
            //                        "name": "Steam",
            //                        "handle": 12345,
            //                        "image": "qrc:/resources/images/controllers/Steam.png",
            //                        "type": 1
            //                    },
            //                    {
            //                        "name": "Steam Deck",
            //                        "handle": 12346,
            //                        "image": "qrc:/resources/images/controllers/Steam Deck.png",
            //                        "type": 2
            //                    }
            //                ]
            model: steam_input.controllers
            TabButton {
                text: modelData.name
                width: implicitWidth
            }
        }
    }

    ListView {
        id: view

        visible: tabBar.count > 0

        Layout.fillWidth: parent
        Layout.fillHeight: parent
        clip: true
        model: tabRepeater.model
        orientation: ListView.Horizontal
        snapMode: ListView.SnapToItem

        highlightRangeMode: ListView.StrictlyEnforceRange

        delegate: GamepadInfoDelegate {
            height: view.height
            width: view.width
        }
    }
    Label {
        id: label
        Layout.fillHeight: parent
        Layout.alignment: Qt.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 48
        text: "No controllers connected"

        visible: tabBar.count == 0
    }
}
