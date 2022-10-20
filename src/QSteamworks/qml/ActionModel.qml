import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15

Model {
    pickable: true

    scale: Qt.vector3d(0.2, 0.2, 0.2)
    source: "#Sphere"

    materials: [ AluminumMaterial {
            bump_amount: 15.0
        }
    ]
}
