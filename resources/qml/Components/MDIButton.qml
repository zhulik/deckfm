import QtQuick.Controls 2.15

ToolButton {
    property string iconName

    focusPolicy: Qt.NoFocus

    MDIIcon {
        name: iconName
        anchors.fill: parent
        anchors.centerIn: parent
    }
}
