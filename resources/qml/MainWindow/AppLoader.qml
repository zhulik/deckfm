import QtQuick 2.15
import QtQuick.Window 2.15

Item {
    id: root
    property alias source: loader.source
    property alias item: loader.item

    property bool showFooter: item
                              && item.deckfmSettings ? item.deckfmSettings['showFooter'] : true

    visible: loader.status == Loader.Ready

    signal closed
    signal loaded

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

            if (item.closed) {
                item.closed.connect(root.close)
            }

            root.loaded()
            if (item.onClosed) {
                item.onClosed.connect(root.close)
            }
        }
    }

    Core.MDIButton {
        anchors.right: parent.right

        iconName: "windowClose"

        visible: item
                 && item.deckfmSettings ? item.deckfmSettings['showCloseButton'] : true

        onClicked: {
            root.close()
        }
    }
}
