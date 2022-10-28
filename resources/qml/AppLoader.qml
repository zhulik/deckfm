import QtQuick 2.15
import QtQuick.Window 2.15
import "./MDI" as MDI

Item {
    id: root
    property alias source: loader.source
    property alias item: loader.item

    visible: loader.status == Loader.Ready

    signal closed()
    signal loaded()

    function close() {
        loader.source = ""
        root.closed()
    }

    onActiveFocusChanged: loader.item.forceActiveFocus()

    Loader {
        id: loader
        asynchronous: true
        anchors.fill: parent

        onLoaded: {
            item.forceActiveFocus()

            item.onVisibleChanged.connect(() => {
                                      if (!item.visible) {
                                          root.close()
                                      }
                                  })

            root.loaded()
        }
    }

    MDI.Button {
        anchors.right: parent.right

        iconName: "windowClose"

        onClicked: {
            root.close()
        }
    }
}
