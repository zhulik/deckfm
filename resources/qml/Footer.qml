import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "QSteamworks" as Steamworks

ToolBar {

    property alias hintActions: repeater.model

    height: 40

    Rectangle {
        anchors.fill: parent

        color: "black"
    }

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: parent
        }

        Repeater {
            id: repeater

            ActionLabel {
                name: modelData
            }
        }
    }
}
