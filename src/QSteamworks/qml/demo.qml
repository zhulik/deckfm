import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12
import "../../../resources/qml/MDI" as MDI

ApplicationWindow {
    id: mainWindow

    Material.theme: Material.Dark

    title: "QSteamworks Demo"
    visible: true

    visibility: { steam_utils.isSteamRunningOnSteamDeck || steam_utils.isSteamInBigPictureMode ? "FullScreen" : "Windowed" }

    width: 1280
    height: 800

    onFrameSwapped: visualView.frameSwapped()

    VisualView {
        id: visualView
        anchors.fill: parent

        RowLayout {
            anchors.right: parent.right

            MDI.Button {
                iconName: "windowClose"
                Layout.alignment: Qt.AlignRight
                onClicked: Qt.quit()
            }
            MDI.Button {
                iconName: "projectorScreen"
                Layout.alignment: Qt.AlignRight
                onClicked: overlay.visible = true
            }
        }

        Rectangle {
            anchors.fill: parent
            id: overlay
            color: "black"
            opacity: 0.5
            visible: false

            ColumnLayout {

                anchors.fill: parent

                RowLayout {

                    TabBar {
                        id: bar
                        Layout.fillWidth: parent

                        TabButton {
                            text: "IGA"
                        }

                        TabButton {
                            text: "Controllers"
                        }
                    }
                    MDI.Button {
                        MDI.Button {
                            iconName: "projectorScreen"
                            Layout.alignment: Qt.AlignRight
                            onClicked: overlay.visible = false
                        }
                    }
                }

                StackLayout {
                    Layout.fillWidth: parent

                    currentIndex: bar.currentIndex

                    IGAView {

                    }

                    ControllerView {

                    }
                }
            }
        }
    }

}
