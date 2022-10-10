import QtQuick.Controls 2.15

ToolButton {
    property string iconName

    Icon {
        name: iconName
        anchors.fill: parent
        anchors.centerIn: parent
    }
}
