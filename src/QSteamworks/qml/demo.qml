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

    header: ToolBar{
        MDI.Button {
            MDI.Button {
                iconName: "windowClose"
                Layout.alignment: Qt.AlignRight
                onClicked: Qt.quit()
            }
        }

    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: bar
            Layout.fillWidth: parent
            currentIndex: 2

            TabButton {
                text: "IGA"
            }

            TabButton {
                text: "Controllers"
            }

            TabButton {
                text: "Visual"
            }
        }

        StackLayout {
            Layout.fillWidth: parent

            currentIndex: bar.currentIndex

            IGAView {

            }

            ControllerView {

            }

            VisualView {

            }
        }
    }

}
