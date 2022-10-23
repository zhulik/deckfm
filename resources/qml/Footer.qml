import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "QSteamworks" as Steamworks

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
            text: `Action set: ${inputScope.actionSet}`
        }
    }

    Steamworks.SteamInputScope {
        id: inputScope
    }
}
