import QtQuick.Controls 2.15

ToolButton {
    property string iconName

    focusPolicy: Qt.NoFocus

    Icon {
        name: iconName
        anchors.fill: parent
        anchors.centerIn: parent
    }
}
