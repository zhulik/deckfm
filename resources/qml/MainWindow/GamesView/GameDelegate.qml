import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../Components" as Core

ItemDelegate {
    id: root

    clip: true

    ColumnLayout {
        anchors.margins: 5
        anchors.fill: parent

        Core.AsyncImage {
            Layout.alignment: Qt.AlignHCenter
            width: parent.width * 0.9
            height: parent.height * 0.8
            source: libraryImagePath
        }

        Label {
            Layout.fillWidth: parent
            text: name
            elide: Text.ElideMiddle
            horizontalAlignment: "AlignHCenter"
        }
    }
}
