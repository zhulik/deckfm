import QtQuick 2.15

Item {
    id: root
    visible: false

    function toggle() {
        root.visible = !root.visible
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.6
    }
}
