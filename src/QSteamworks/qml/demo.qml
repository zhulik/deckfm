import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

ApplicationWindow {
    id: mainWindow

    Material.theme: Material.Dark

    title: "QSteamworks Demo"
    visible: true

    visibility: { steam_utils.isSteamRunningOnSteamDeck || steam_utils.isSteamInBigPictureMode ? "FullScreen" : "Windowed" }

    width: 1280
    height: 800

    ColumnLayout {
        anchors.fill: parent

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

        StackLayout {
            Layout.fillWidth: parent

            currentIndex: bar.currentIndex

            IGAView {

            }

            Item {
                id: discoverTab
            }
        }
    }

}
