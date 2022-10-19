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

        function v3dSum() { // multiple Vetctor3D arguments
            let [x, y, z] = [0 , 0, 0]

            for (var i = 0; i < arguments.length; i++) {
                x += arguments[i].x
                y += arguments[i].y
                z += arguments[i].z
            }
            return Qt.vector3d(x, y, z)
        }

        function v3dMulC(vec, n) { // multiple Vetctor3D arguments
            return Qt.vector3d(vec.x * n, vec.y * n, vec.z * n);
        }

        function move(dX, dY) {
            var forwardVelocity = v3dMulC(camera.forward, dY * 3)
            var strafeVelocity = v3dMulC(camera.right, dX * 3)
            position = v3dSum(position, forwardVelocity, strafeVelocity)
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

