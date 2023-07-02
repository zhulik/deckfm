import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Pane {
    id: root
    property alias text: headerLabel.text

    default property alias content: inner.data

    property bool collapsed: false


    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: parent
            Button {
                text: ">"

                onClicked: {
                    root.collapsed = !root.collapsed
                    console.log(root.collapsed)
                }
            }

            Label {
                id: headerLabel
            }
        }

        Item {
            id: inner
            visible: !root.collapsed

            //        clip: true
            Layout.preferredHeight: {
                Object.values(children).reduce((acc, c) => { return acc + c.height }, 0)
            }

        }
    }
}
