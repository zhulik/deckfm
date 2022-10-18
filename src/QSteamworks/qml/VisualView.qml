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

             PerspectiveCamera {
                 id: camera
                 position: Qt.vector3d(0, 200, 300)
                 eulerRotation.x: -30
             }

             DirectionalLight {
                 eulerRotation.x: -30
             }

             SteamInputScope {
                 events: ["RightPad", "RightPad"]

                 onInputEvent: {
                     rightSphere.rotation.x += event.analogX
                     rightSphere.rotation.y += event.analogY
//                     logLabel.text = JSON.stringify(event, null, 2)
                 }
             }

             Model {
                 id: rightSphere
                 visible: true
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

//                 SequentialAnimation on eulerRotation {
//                     loops: Animation.Infinite
//                     PropertyAnimation {
//                         duration: 5000
//                         from: Qt.vector3d(0, 0, 0)
//                         to: Qt.vector3d(360, 0, 360)
//                     }
//                 }
             }
         }
}
