import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15
import QtQuick3D.Helpers 1.15

import "../../../resources/qml/MDI" as MDI


View3D {
    id: root
    anchors.fill: parent
    camera: camera
    renderMode: View3D.Inline

    property bool enableDebug: false

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

    DebugView {
        source: root
        visible: enableDebug
    }

    AxisHelper {
        visible: enableDebug
    }

    FlyingCamera {
        id: camera
        position: Qt.vector3d(0, 30, 0)

        onPositionChanged: {
            if (position.y != 30) {
                position.y = 30
            }
        }
    }

    Model {
        id: ground
        position: Qt.vector3d(0, 0, 0)
        source: "#Cylinder"
        scale: Qt.vector3d(2, 0.1, 2)
        materials: [ PrincipledMaterial {
                metalness: 0

                baseColorMap: Texture {
                    source: "ground_diff.jpg"
                    tilingModeHorizontal: Texture.Repeat
                    tilingModeVertical: Texture.Repeat
                    scaleU: 8
                    scaleV: 8
                }
                roughnessMap: Texture { source: "maps/metallic/ground_rough.jpg" }
                normalMap: Texture { source: "maps/metallic/ground_normal.jpg" }

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

    Model {
        position: Qt.vector3d(100, 20, 0)
        source: "#Sphere"
        scale: Qt.vector3d(0.2, 0.2, 0.2)
        materials: [ SteelMilledConcentricMaterial {} ]
    }

    Model {
        position: Qt.vector3d(-100, 20, 0)
        scale: Qt.vector3d(0.2, 0.2, 0.2)
        source: "#Sphere"
        materials: [ GlassRefractiveMaterial {} ]
    }

    Model {
        position: Qt.vector3d(0, 20, 100)
        scale: Qt.vector3d(0.2, 0.2, 0.2)
        source: "#Sphere"
        materials: [ PlasticStructuredRedMaterial {
                material_ior: 8
                bump_factor: 10
            }
        ]
    }

    Model {
        position: Qt.vector3d(0, 20, -100)
        scale: Qt.vector3d(0.2, 0.2, 0.2)
        source: "#Sphere"
        materials: [ AluminumMaterial {
                bump_amount: 15.0
            }
        ]
    }

    CameraInfo {
        camera: camera
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        visible: enableDebug
    }



    RowLayout {
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        MDI.Button {
            iconName: "axis"
            Layout.alignment: Qt.AlignRight
            onClicked: root.enableDebug = !root.enableDebug
        }
    }
}

