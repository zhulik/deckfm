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
                if (event.name === "LeftPad") {
                    sphere = leftSphere
                } else {
                    sphere = rightSphere
                }

                sphere.eulerRotation.y += event.analogX / 5
                sphere.eulerRotation.z += event.analogY / 5
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
