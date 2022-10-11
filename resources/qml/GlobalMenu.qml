import QtQuick 2.15
import QtQuick.Controls 2.15

import "./MDI" as MDI

Menu {
    id: root

    modal: true

    signal exitClicked

    anchors.centerIn: parent

    onOpened: {
        exitItem.forceActiveFocus()
    }

    MDI.MenuItem {
        iconName: "windowClose"
        itemText: "Exit"
    }

    MDI.MenuItem {
        iconName: "windowClose"
        itemText: "Exit"
    }

    MenuSeparator {}

    MDI.MenuItem {
        id: exitItem
        iconName: "windowClose"
        itemText: "Exit"

        onTriggered: root.exitClicked()
    }
}
