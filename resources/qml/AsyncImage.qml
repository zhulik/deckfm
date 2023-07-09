import QtQuick 2.15
import QtQuick.Controls 2.15

Image {
    id: root

    fillMode: Image.PreserveAspectFit
    asynchronous: true

    BusyIndicator {
        anchors.fill: parent
        visible: root.status === Image.Loading
    }
}
