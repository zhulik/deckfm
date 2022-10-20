import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15

Model {
    pickable: true

    property bool picked: false

    readonly property real scaleF: { 0.2 * (picked ? 1.2 : 1) }

    scale: Qt.vector3d(scaleF, scaleF, scaleF)
    source: "#Sphere"

    materials: [ AluminumMaterial {
            bump_amount: 15.0
        }
    ]

    function pick() {
        picked = true
    }

    function unpick() {
        picked = false
    }
}
