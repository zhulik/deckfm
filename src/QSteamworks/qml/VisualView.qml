import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15


Item {
    id: root

    View3D {
        id: view
        anchors.fill: parent
        camera: camera
        renderMode: View3D.Overlay
        visible: root.visible

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 000, 200)
        }

        DirectionalLight {
            eulerRotation.x: -30
        }

        SteamInputScope {
            events: ["LeftPad", "RightPad"]

            onInputEvent: {
                rightSphere.eulerRotation.y += event.analogX / 5
                rightSphere.eulerRotation.z += event.analogY / 5
                //                     logLabel.text = JSON.stringify(event, null, 2)
            }
        }

        Model {
            id: rightSphere
            //                 visible: root.visible
            position: Qt.vector3d(0, 0, 0)
            source: "#Sphere"
            materials: [ DefaultMaterial {
                    diffuseMap: Texture {
                        id: texture
                        source: "./right.png"
                    }
                }
            ]
            eulerRotation.y: 90
        }
    }
}
