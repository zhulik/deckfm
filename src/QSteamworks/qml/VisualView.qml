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
                let sphere;

                switch(event.action.actionDefinition.name) {
                case "LeftPad":
                    leftSphere.eulerRotation.y += event.analogX
                    leftSphere.eulerRotation.z -= event.analogY / 5
                    break
                case "RightPad":
                    rightSphere.eulerRotation.y += event.analogX / 5
                    rightSphere.eulerRotation.z += event.analogY / 5
                    break
                }
            }
        }

        Model {
            id: rightSphere
            position: Qt.vector3d(100, 0, 0)
            source: "#Sphere"
            materials: [ DefaultMaterial {
                    diffuseMap: Texture {
                        source: "./right.png"
                    }
                }
            ]
            eulerRotation.y: 90
        }

        Model {
            id: leftSphere
            position: Qt.vector3d(-100, 0, 0)
            source: "#Sphere"
            materials: [ DefaultMaterial {
                    diffuseMap: Texture {
                        source: "./right.png"
                    }
                }
            ]
            eulerRotation.y: 90
        }
    }
}
