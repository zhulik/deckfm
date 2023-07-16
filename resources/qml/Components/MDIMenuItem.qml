import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15

Controls.MenuItem {
    id: root

    property string iconName
    property string itemText

    MDIIcon {
        leftPadding: 15
        name: iconName
        anchors.verticalCenter: parent.verticalCenter
    }

    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Space:
        case Qt.Key_Return:
            event.accepted = true
            root.clicked()
        }
    }

    Controls.Label {
        text: itemText
        anchors.centerIn: parent
        font.pointSize: 12
    }
}
