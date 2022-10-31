import QtQuick 2.15
import QtQuick.Layouts 1.15

import "QSteamworks" as Steamworks

RowLayout {
    id: root

    property string name

    Steamworks.ActionIcon {
        Layout.fillHeight: parent

        name: root.name
    }

    Steamworks.ActionText {
        Layout.fillHeight: parent
        verticalAlignment: Text.AlignVCenter

        font.pointSize: 18

        name: root.name
    }
}
