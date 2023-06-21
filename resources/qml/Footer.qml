import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "QSteamworks" as Steamworks

ToolBar {
    id: root

    property alias hintActions: repeater.model

    height: 20

    Rectangle {
        anchors.fill: parent

        color: "black"
    }

    RowLayout {
        spacing: 10
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 11

        Item {
            Layout.fillWidth: parent
        }

        Repeater {
            id: repeater

            ActionLabel {
                height: root.height
                name: modelData
            }
        }
    }
}
