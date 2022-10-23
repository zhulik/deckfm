import QtQuick 2.15
import QtQuick.Controls 2.15

import "./MDI" as MDI

Menu {
    id: root

    modal: true

    signal exitClicked

    anchors.centerIn: parent

    MDI.MenuItem {
        id: exitItem
        iconName: "windowClose"
        itemText: "Exit"

        onTriggered: root.exitClicked()
    }
}
