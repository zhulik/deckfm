import QtQuick 2.15

Text {
    id: root
    property int frameCounter: 0
    property int frameCounterAvg: 0
    property int counter: 0
    property int fps: 0
    property int fpsAvg: 0

    color: "white"
    text: "FPS:" + root.fpsAvg + " | " + root.fps + " fps"

    Image {
        NumberAnimation on rotation {
            from:0
            to: 360
            loops: Animation.Infinite
        }
        onRotationChanged: frameCounter++;
    }


    Timer {
        interval: 500
        repeat: true
        running: true

        readonly property double k: (1000/interval)

        onTriggered: {
            frameCounterAvg += frameCounter;
            root.fps = frameCounter*k;
            counter++;
            frameCounter = 0;
            if (counter >= 3) {
                root.fpsAvg = frameCounterAvg/(counter/k)
                frameCounterAvg = 0;
                counter = 0;
            }
        }
    }
}
