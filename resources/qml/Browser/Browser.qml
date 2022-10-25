import QtQuick 2.15
import QtWebEngine 1.10

import "../QSteamworks" as Steamworks

Item {
    id: root

    WebEngineView {
        id: webView
        anchors.fill: parent

        url: "https://stackoverflow.com/"
    }

//    Steamworks.SteamInputScope {
//        enabled: root.activeFocus

//        analogHandlers: {
//            "browser_scroll": (e) => webView.runJavaScript(`window.scrollBy(${e.analogX}, ${e.analogY})`)
//        }

//        actionSet: "browser_navigation"
//    }
}
