import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property bool cached: false

    property string source

    Image {
        id: image
        anchors.fill: parent

        fillMode: Image.PreserveAspectFit
        asynchronous: true

        sourceSize.width: image.width
        sourceSize.height: image.height

        source: root.cached ? `image://cache_thumbnail/${root.source}` : root.source
    }

    BusyIndicator {
        anchors.fill: parent
        visible: image.status === Image.Loading
    }
}
