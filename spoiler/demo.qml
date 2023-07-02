import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Rectangle {
    color: "#2E2F30"

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.preferredWidth: parent.width / 2

            Label {
                Layout.fillWidth: parent
                text: spoiler.height
            }
        }

        ColumnLayout {
            Layout.preferredWidth: parent.width / 2

            Spoiler {
                id: spoiler
                Layout.fillWidth: parent

                text: "Some text"

                ColumnLayout {
                    Layout.fillWidth: parent

                    Slider {
                        Layout.fillWidth: parent
                    }

                }
            }
        }
    }

}
