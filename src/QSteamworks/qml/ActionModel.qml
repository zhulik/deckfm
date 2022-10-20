import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15

Model {
    pickable: true

    property bool picked: false

    readonly property real scaleF: 0.2

    scale: Qt.vector3d(scaleF, scaleF, scaleF)
    source: "#Sphere"

    function pick() {
        picked = true
    }

    function unpick() {
        picked = false
    }

    SequentialAnimation on eulerRotation.y {
        loops: Animation.Infinite
        running: picked

        NumberAnimation {
            duration: 3000
            to: 0
            from: 359
        }
    }
}
