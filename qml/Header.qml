import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "./MDI" as MDI

ToolBar {
    id: root

    signal menuClicked
    signal logoClicked
    signal exitClicked

    RowLayout {
        anchors.fill: parent

        MDI.Button {
            iconName: "menu"
            onClicked: root.menuClicked()
        }

        ToolSeparator {}

        ToolButton {
            text: system_bridge.isOnDeck ? "DECKFM" : "DFM"
            font.pointSize: 24
            Layout.fillWidth: parent
            Layout.maximumWidth: 150

            onClicked: root.logoClicked()
        }

        ToolButton {
            text: steamworks_bridge.username
            font.pointSize: 24
            Layout.fillWidth: parent
            Layout.maximumWidth: 400

            onClicked: steamworks_bridge.show_overlay()
        }

        Item {
            Layout.fillWidth: parent
        }

        ToolSeparator {}

        MDI.Button {
            iconName: "windowClose"
            Layout.alignment: Qt.AlignRight
            onClicked: root.exitClicked()
        }
    }
}
