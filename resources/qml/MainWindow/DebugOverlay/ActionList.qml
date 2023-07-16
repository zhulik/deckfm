import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.12

import "../../Components" as Core

ListView {
    id: root
    clip: true

    property var actions: []

    model: Core.JSONListModel {
        data: root.actions
    }

    delegate: ItemDelegate {
        width: root.width
        height: 50

        RowLayout {
            anchors.fill: parent

            Label {
                Layout.fillWidth: parent
                height: parent.height
                font.pointSize: 20

                text: name
                verticalAlignment: Qt.AlignVCenter
            }
        }
    }
}
