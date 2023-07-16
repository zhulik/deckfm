import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../Components" as Core

ToolBar {
    id: root

    property alias hintActions: repeater.model

    height: 20

    Rectangle {
        anchors.fill: parent

        color: "black"
    }

    RowLayout {
        spacing: 20
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Item {
            Layout.fillWidth: parent
        }

        Repeater {
            id: repeater

            Core.ActionLabel {
                height: root.height
                name: modelData
            }
        }
    }
}
