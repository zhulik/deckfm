import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "../MDI" as MDI

ItemDelegate {
    enabled: fileReadable
    clip: true

    RowLayout {
        anchors.fill: parent

        MDI.Icon {
            name: fileIcon
            Layout.fillHeight: parent
        }

        ColumnLayout {
            Layout.fillWidth: parent

            Label {
                text: fileName
                Layout.fillWidth: parent
                font.pointSize: 12
                elide: Text.ElideMiddle
            }

            Label {
                text: fileSizeString
                Layout.fillWidth: parent
                color: Material.accent
                font.pointSize: 8
            }
        }

        Label {
            text: fileMime
            elide: Text.ElideMiddle
            color: Material.accent
            Layout.alignment: Qt.AlignVCenter
            font.pointSize: 8
        }
    }
}
