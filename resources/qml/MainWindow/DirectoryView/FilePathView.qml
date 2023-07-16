import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

import "../Components" as Core

ListView {
    id: root
    property var pathComponents

    signal pathSelected(string path)

    LinearGradient {
        height: parent.height
        width: atXBeginning ? 0 : 150

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutBounce
            }
        }

        start: Qt.point(0, 0)
        end: Qt.point(150, 0)

        gradient: Gradient {
            GradientStop {
                position: 0.2
                color: mainWindow.color // FIXME: Why Material.background draws white?
            }
            GradientStop {
                position: 1.0
                color: "transparent"
            }
        }
    }

    clip: true
    orientation: ListView.Horizontal

    onCountChanged: currentIndex = count - 1

    remove: Transition {
        id: _transition

        NumberAnimation {
            property: "opacity"
            from: 1
            to: 0
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }

    model: Core.JSONListModel {
        id: listModel
        data: pathComponents
    }

    header: Core.MDIButton {
        iconName: "harddisk"
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            pathSelected("/")
        }
    }

    delegate: ItemDelegate {
        readonly property bool isLast: index + 1 == listModel.count

        width: label.width
        height: root.height
        focusPolicy: Qt.NoFocus

        onClicked: {
            root.pathSelected(path)
        }

        Rectangle {
            anchors.fill: label
            anchors.margins: 5
            color: Material.accent
            opacity: 0.7
            visible: isLast
        }

        Label {
            id: label
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            padding: {
                left: 10
                right: 10
            }

            font.pointSize: 16
            text: name
        }
    }

    section.property: "path"
    section.delegate: Label {
        height: parent.height
        font.pointSize: 16
        text: "/"
        verticalAlignment: Text.AlignVCenter
    }
}
