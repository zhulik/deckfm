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
            position: Qt.vector3d(0, 000, 300)
            eulerRotation.x: 20
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
                    leftSphere.eulerRotation.z -= event.analogY
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
            position: Qt.vector3d(200, 0, 0)
            source: "#Sphere"
            materials: [ DefaultMaterial {

                    diffuseMap: Texture {
                        source: "./sphere.png"
                        scaleU: 2
                        scaleV: 1
                    }
                }
            ]
            eulerRotation.y: 90
        }

        Model {
            id: leftSphere
            position: Qt.vector3d(-200, 0, 0)
            source: "#Sphere"
            materials: [ DefaultMaterial {
                    diffuseMap: Texture {
                        scaleU: 2
                        scaleV: 1
                        source: "./sphere.png"
                    }
                }
            ]
            eulerRotation.y: 90
        }
    }
}
