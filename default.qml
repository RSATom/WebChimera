import QtQuick 2.1
import QtMultimedia 5.0

Rectangle {
    color: bgcolor
    VideoOutput {
        id: videoOutput
        source: vlcPlayer
        anchors.fill: parent
    }
    MouseArea {
        anchors.fill: videoOutput
        onClicked: vlcPlayer.togglePause();
    }
}
