import QtQuick 2.15
import QtQuick.Window 2.15
import "./MDI" as MDI

Item {
    property alias source: loader.source

    visible: source != ""

    Loader {
        id: loader
        asynchronous: true
        anchors.fill: parent
    }

    MDI.Button {
        anchors.right: parent.right

        iconName: "windowClose"

        onClicked: {
            loader.source = ""
        }
    }
}
