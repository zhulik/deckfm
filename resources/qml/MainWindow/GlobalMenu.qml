import QtQuick 2.15
import QtQuick.Controls 2.15

import "../Components" as Core

Menu {
    id: root

    modal: true

    signal exitClicked

    anchors.centerIn: parent

    Core.MDIMenuItem {
        id: exitItem
        iconName: "windowClose"
        itemText: "Exit"

        onTriggered: root.exitClicked()
    }
}
