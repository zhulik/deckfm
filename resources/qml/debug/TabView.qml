import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ColumnLayout {
    id: root

    property var model: []
    property var selectedItem: model[tabBar.currentIndex]

    property alias tabDelegate: tabRepeater.delegate
    property alias delegate: stackRepeater.delegate


    TabBar {
        id: tabBar
        Layout.fillWidth: parent

        Repeater {
            id: tabRepeater
            anchors.fill: parent
            model: root.model
        }
    }

    StackLayout {
        id: stack
        Layout.fillWidth: parent
        Layout.fillHeight: parent

        currentIndex: tabBar.currentIndex

        Repeater {
            id: stackRepeater
            anchors.fill: parent

            model: root.model
        }
    }
}
