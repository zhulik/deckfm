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

       Steamworks.ActionIcon {
           Layout.fillHeight: parent

           name: "folder_activate"
       }

       Steamworks.ActionLabel {
           Layout.fillHeight: parent

           name: "folder_activate"
       }
    }

    Steamworks.SteamInputScope {
        id: inputScope
    }
}
