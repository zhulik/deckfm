import QtQuick 2.15
import QtQuick.Controls 2.15

import "./MDI" as MDI

Menu {
    id: root

    modal: true

    signal exitClicked
    signal gamepadClicked

    anchors.centerIn: parent

    MDI.MenuItem {
        iconName: "gamepad"
        itemText: "Gamepads"
        onTriggered: root.gamepadClicked()
    }

    MenuSeparator {}

    MDI.MenuItem {
        id: exitItem
        iconName: "windowClose"
        itemText: "Exit"

        onTriggered: root.exitClicked()
    }
}
