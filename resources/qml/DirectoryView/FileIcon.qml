import QtQuick 2.15

import "../MDI" as MDI
import ".." as Core

Item {
    id: root

    property string name
    property string mime
    property string path

    readonly property bool isImage: root.mime.startsWith("image/")

    MDI.Icon {
        id: icon
        anchors.fill: parent

        name: root.name
        visible: !root.isImage
    }

    Core.AsyncImage {
        anchors.fill: parent
        source: path
        cached: true
        visible: root.isImage
    }
}
