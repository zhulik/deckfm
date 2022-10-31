import QtQuick 2.15
import QtQuick.Layouts 1.15

import "QSteamworks" as Steamworks

RowLayout {
    id: root
    spacing: 20

    property string name

    Steamworks.ActionIcon {
        Layout.fillHeight: parent

        name: root.name
    }

    Steamworks.ActionText {
        Layout.fillHeight: parent
//        Layout.alignment: Qt.AlignVCenter

//        font.pointSize: root.height - 5

        name: root.name
    }
}
