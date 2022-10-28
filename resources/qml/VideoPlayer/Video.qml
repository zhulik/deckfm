import QtQuick 2.15

import QtMultimedia 5.15 as M

import DeckFM 1.0

Item {
    property string name: FSHelpers.fileName(source)

    property alias source: video.source
    property alias position: video.position
    property alias duration: video.duration
    property alias playbackState: video.playbackState

    property alias errorString: video.errorString

    function play() {
        video.play()
    }

    function pause() {
        video.pause()
    }

    function playPause()  {
        if (playbackState == M.MediaPlayer.PlayingState) {
           pause()
        } else {
            play()
        }
    }

    function seek(position, debounce = true) {
        const now = new Date()

         if (now - video.limitStartTime >= 200 | !debounce) {
             video.limitStartTime = now
             video.seek(position)
         }
    }

    M.Video {
        id: video

        property var limitStartTime: new Date()

        anchors.fill: parent
        autoPlay: true
    }
}
