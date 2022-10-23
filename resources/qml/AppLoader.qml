import QtQuick 2.15
import QtQuick.Window 2.15
import "./MDI" as MDI

Item {
    id: root
    property alias source: loader.source

    visible: loader.status == Loader.Ready

    signal closed()
    signal loaded()

    onActiveFocusChanged: loader.item.forceActiveFocus()

    Loader {
        id: loader
        asynchronous: true
        anchors.fill: parent

        onLoaded: {
            item.forceActiveFocus()
            root.loaded()
        }
    }

    MDI.Button {
        anchors.right: parent.right

        iconName: "windowClose"

        onClicked: {
            loader.source = ""
            root.closed()
        }
    }
}
