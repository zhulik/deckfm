import QtQuick 2.15
import QtQuick.Window 2.15
import "./MDI" as MDI

Item {
    id: root
    property alias source: loader.source
    property alias item: loader.item

    property bool showFooter: item && item.deckfmSettings ? item.deckfmSettings['showFooter'] : true

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

            if(item.closed) {
                item.closed.connect(root.close)
            }

            root.loaded()

            item.onClosed.connect(root.close)
        }
    }

    MDI.Button {
        anchors.right: parent.right

        iconName: "windowClose"

        visible: item && item.deckfmSettings ? item.deckfmSettings['showCloseButton'] : true


        onClicked: {
            root.close()
        }
    }
}
