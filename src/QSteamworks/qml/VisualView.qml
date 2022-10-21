import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick3D 1.15
import QtQuick3D.Materials 1.15
import QtQuick3D.Helpers 1.15

import "../../../resources/qml/MDI" as MDI


View3D {
    id: root

    property bool enableDebug: false
    property var pickedObject: null

    anchors.fill: parent
    renderMode: View3D.Inline

    camera: camera

    function frameSwapped() {
        doPick()
    }

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox

        lightProbe: Texture {
            source: "sky.hdr"
        }

        antialiasingMode: SceneEnvironment.SSAA
        antialiasingQuality: SceneEnvironment.VeryHigh
    }

    Rectangle {
        anchors.centerIn: parent

        width: pickedObject ? 30 : 18
        height: width
        radius: width / 2
        opacity: 0.5
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

    function doPick() {
        const result = pick(root.width / 2, root.height / 2)

        if (pickedObject === result.objectHit) {
            return
        }

        if (result.distance < 40 && result.objectHit) {
            if (pickedObject) {
                pickedObject.unpick()
            }

            pickedObject = result.objectHit

            pickedObject.pick()
        } else {
            if (pickedObject) {
                pickedObject.unpick()
            }
            pickedObject = null
        }
    }

    FlyingCamera {
        id: camera

        position: Qt.vector3d(0, 30, -70)

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
        id: input

        handlers: {
            "LeftStick": (e) => camera.move(e.analogX, e.analogY),
            "RightPad": (e) => camera.pan(e.analogX / 5, e.analogY / 5),
            "touch_rstick": (e) => {
                if (pickedObject) {
                    pickedObject.animated = !e.digitalState
                }
            },
            "menu": (e) => {
                if (e.digitalState) {
                    overlay.visible = !overlay.visible
                }
            },
            "select": (e) => {
                if (e.digitalState) {
                    Qt.quit()
                }
            },
            "RightStick": (e) => {
                if (pickedObject) {
                    pickedObject.rotate(e.analogX / 15, Qt.vector3d(0, 1, 0), Node.SceneSpace)
                    pickedObject.rotate(e.analogY / 15, Qt.vector3d(1, 0, 0), Node.SceneSpace)

                }
            }
        }
    }

    ColumnLayout {
        anchors.bottom: parent.bottom

        Slider {
            id: hapticsIntencity
            from: 0
            value: 0
            to: 65000
            stepSize: 2000
        }

        Slider {
            id: hapticsOtherIntencity
            from: 0
            value: 0
            to: 65000
            stepSize: 2000
        }

        Slider {
            id: hapticsGaindb

            from: -127
            value: 0
            to: 127
            stepSize: 10
        }

        Slider {
            id: hapticsOtherGainDB

            from: -127
            value: 0
            to: 127
            stepSize: 10
        }

        Timer {
            interval: 100
            repeat: true
            running: true

            onTriggered: {
                steam_input.triggerSimpleHapticEvent("both", hapticsIntencity.value,
                                                     hapticsGaindb.value,
                                                     hapticsOtherIntencity.value,
                                                     hapticsOtherGainDB.value)
            }
        }
    }



    ActionModel {
        position: Qt.vector3d(100, 30, 0)
        materials: [ SteelMilledConcentricMaterial {} ]
    }

    ActionModel {
        position: Qt.vector3d(-100, 30, 0)
        materials: [ GlassRefractiveMaterial {} ]
    }

    ActionModel {
        position: Qt.vector3d(0, 30, 100)
        materials: [ PlasticStructuredRedMaterial {
                material_ior: 8
                bump_factor: 10
            }
        ]
    }

    ActionModel {
        position: Qt.vector3d(0, 30, -100)

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

    Rectangle {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        width: 10
        height: 10

        SequentialAnimation on rotation {
            loops: Animation.Infinite
            running: picked

            NumberAnimation {
                duration: 3000
                to: 0
                from: 359
            }
        }
    }
}

