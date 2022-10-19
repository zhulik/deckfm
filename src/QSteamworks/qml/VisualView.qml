import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15
import QtQuick3D.Helpers 1.15


View3D {
    id: root
    anchors.fill: parent
    camera: camera
    visible: root.visible

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox

        lightProbe: Texture {
            source: "sky.hdr"
        }

        antialiasingMode: SceneEnvironment.SSAA
        antialiasingQuality: SceneEnvironment.VeryHigh
    }

    WasdController {
        controlledObject: camera
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 300, 0)

        function move(dX, dY) {
            const fdir = camera.forward

            var forwardVelocity = Qt.vector3d(fdir.x * dY * 3,
                                       fdir.y * dY * 3,
                                       fdir.z * dY * 3);

            const sdir = camera.right

            var strafeVelocity = Qt.vector3d(sdir.x * dX * 3,
                                       sdir.y * dX * 3,
                                       sdir.z * dX * 3);

            position = Qt.vector3d(position.x + forwardVelocity.x + strafeVelocity.x,
                                   position.y + forwardVelocity.y + strafeVelocity.y,
                                   position.z + forwardVelocity.z + strafeVelocity.z);
        }

        onEulerRotationChanged: {
            if (camera.eulerRotation.x > 90) {
                camera.eulerRotation.x = 90
            }

            if (camera.eulerRotation.x < -90) {
                camera.eulerRotation.x = -90
            }
        }

        function pan(dX, dY) {
            camera.eulerRotation.y -= dX
            camera.eulerRotation.x -= dY

            if (Math.abs(camera.eulerRotation.x) > 90) {
                camera.eulerRotation.x += dY
            }

        }
    }

    Model {
        id: ground
        position: Qt.vector3d(0, 0, 0)
        source: "#Cylinder"
        scale: Qt.vector3d(4, 0.1, 4)
        materials: [ DefaultMaterial {
                diffuseColor: "green"
            }
        ]
    }

    SteamInputScope {
        events: ["LeftStick", "RightPad", "select", "menu"]

        onInputEvent: {
            switch(event.action.actionDefinition.name) {
            case "LeftStick":
                camera.move(event.analogX, event.analogY)
                break
            case "RightPad":
                camera.pan(event.analogX / 5, event.analogY / 5)
                break
                //            case "select":
                //                Qt.quit()
                //                break
            case "menu":
                if (event.digitalState) {
                    overlay.visible = !overlay.visible
                }
                break
            }
        }
    }

    //        Model {
    //            id: rightSphere
    //            position: Qt.vector3d(0, 0, 0)
    //            source: "#Sphere"
    //            materials: [ SteelMilledConcentricMaterial {

    //                }
    //            ]
    //        }
}

