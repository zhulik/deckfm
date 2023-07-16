import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

ItemDelegate {
    enabled: fileReadable
    clip: true

    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        FileIcon {
            Layout.fillHeight: parent
            Layout.preferredWidth: 50

            name: fileIcon
            mime: fileMime
            path: filePath
        }

        ColumnLayout {
            Layout.fillWidth: parent

            Label {
                text: fileName
                Layout.fillWidth: parent
                font.pointSize: 12
                elide: Text.ElideMiddle
            }

            RowLayout {

                Label {
                    text: fileSizeString
                    Layout.fillWidth: parent
                    color: Material.accent
                    font.pointSize: 8
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
    }
}
