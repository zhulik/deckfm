import QtQuick 2.15

Item {
    id: root

    property alias target: connections.target
    property alias enabled: connections.enabled

    readonly property point rightStick: gamepad.rightStick
    readonly property point leftStick: gamepad.leftStick

    default property var repeatButtons: new Array

    Connections {
        id: connections
        target: gamepad
        enabled: root.parent.activeFocus
    }
}
