import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15



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

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        property int prevX: -1
        property int prevY: -1

        signal mouseMoved(var delta)

        onPositionChanged: {
            if (prevX != -1) {
                const dX = mouse.x - prevX
                const dY = mouse.y - prevY

                if (dX != 0 || dY != 0) {
                    mouseArea.mouseMoved({x: dX, y: dY})
                }
            }
            prevX = mouse.x
            prevY = mouse.y
        }
    }

    Connections {
        target: mouseArea

        function onMouseMoved(delta) {
            camera.pan(delta.x, delta.y)
        }
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 300, 0)

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
                camera.x += event.analogX
                camera.y += event.analogY
                break
            case "RightPad":
                camera.pan(event.analogX / 5, event.analogY / 5)
                break
            case "select":
                Qt.quit()
                break
            case "menu":
                overlay.visible = !overlay.visible
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

