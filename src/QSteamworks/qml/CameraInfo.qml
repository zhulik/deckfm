import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    property var camera

    width: layout.width + 20
    height: layout.height + 10

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.3
    }


    ColumnLayout {
        id: layout

        Label {
            text: `Position: ${camera.position}`
        }

        Label {
            id: direction
            text: `Direction: ${camera.forward}`
        }

        Connections {
            target: camera

            function onEulerRotationChanged(r) {
                direction.text = `Direction: ${camera.forward}`
            }
        }
    }
}
