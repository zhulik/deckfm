import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

import "../MDI" as MDI

ListView {
    id: root
    property string path

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

    onPathChanged: {
        const parts = fs_bridge.split_path(path)

        if (parts.length < listModel.count) {
            listModel.remove(parts.length, listModel.count - parts.length)
        }

        parts.forEach(function (p, i) {
            const existing = listModel.get(i)
            if (existing) {
                listModel.set(i, p)
            } else {
                listModel.insert(i, p)
            }
        })
    }

    onCountChanged: {
        currentIndex = count - 1
    }

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

    model: ListModel {
        id: listModel
    }

    header: MDI.Button {
        iconName: "harddisk"
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            pathSelected("file:///")
        }
    }

    delegate: ItemDelegate {
        readonly property bool isLast: index + 1 == listModel.count

        width: label.width
        height: root.height

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
