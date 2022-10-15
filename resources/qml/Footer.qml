import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ToolBar {
    height: 40
    Rectangle {
        anchors.fill: parent

        color: "black"
    }

    RowLayout {
        anchors.fill: parent

        Label {
            font.pointSize: 24
            text: `Action set: ${steam_input.actionSet}`
        }
    }
}
