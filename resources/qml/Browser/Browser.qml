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

    Steamworks.SteamInputScope {
        enabled: root.activeFocus

        property point prevScroll

        analogHandlers: {
            "browser_scroll": (e) => {
                const dx = Math.abs(e.analogX)
                const dy = Math.abs(e.analogY)

                if (prevScroll.x !== dx || prevScroll.y !== dy) {
                    webView.runJavaScript(`window.scrollBy(${dx}, ${dy})`)
                    prevScroll = Qt.point(dx, dy)
                }
            }
        }
        actionSet: "browser_navigation"
    }
}
