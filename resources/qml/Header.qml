import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "./MDI" as MDI

ToolBar {
    id: root

    signal menuClicked
    signal logoClicked
    signal gamepadClicked
    signal exitClicked

    RowLayout {
        anchors.fill: parent

        MDI.Button {
            iconName: "menu"
            onClicked: root.menuClicked()
        }

        ToolButton {
            text: "DECKFM"
            font.pointSize: 24
            onClicked: root.logoClicked()
        }

        Label {
            font.pointSize: 24
            text: `Action set: ${steam_input.actionSet}`
        }

        Item {
            Layout.fillWidth: parent
        }

        MDI.Button {
            iconName: "gamepad"
            Layout.alignment: Qt.AlignRight
            onClicked: root.gamepadClicked()
        }

        ToolSeparator {}

        MDI.Button {
            iconName: "windowClose"
            Layout.alignment: Qt.AlignRight
            onClicked: root.exitClicked()
        }
    }
}
