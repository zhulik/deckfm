import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Components" as Core

RowLayout {
    id: root

    property string name

    Core.ActionIcon {
        Layout.fillHeight: parent

        name: root.name
    }

    Core.ActionText {
        Layout.fillHeight: parent

        name: root.name
    }
}
