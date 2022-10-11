import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "../MDI" as MDI

ItemDelegate {
    property var model

    enabled: model.canCd(filePath)
    clip: true

    RowLayout {
        anchors.fill: parent

        MDI.Icon {
            name: model.icon(filePath)
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
                text: model.fileSizeStr(filePath)
                Layout.fillWidth: parent
                color: Material.accent
                font.pointSize: 8
            }
        }

        Label {
            text: model.mime(filePath).join("/")
            elide: Text.ElideMiddle
            color: Material.accent
            Layout.alignment: Qt.AlignVCenter
            font.pointSize: 8
        }
    }
}
