import QtQuick 2.15
import QtQuick.Layouts 1.15

RowLayout {
    id: root

    property bool playing: false
    property bool hasPrevious: false
    property bool hasNext: false

    signal previousPressed
    signal playPausePressed
    signal nextPressed

    Core.MDIButton {
        Layout.fillHeight: parent
        iconName: "skipPrevious"
        visible: hasPrevious
        onPressed: root.previousPressed()
    }

    Core.MDIButton {
        Layout.fillHeight: parent
        iconName: playing ? "pause" : "play"
        onPressed: root.playPausePressed()
    }

    Core.MDIButton {
        Layout.fillHeight: parent
        iconName: "skipNext"
        visible: hasNext
        onPressed: root.nextPressed()
    }
}
