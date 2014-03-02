import QtQuick 2.1
import QmlVlc 0.1

Rectangle {
    color: bgcolor
    VlcVideoSurface {
        id: videoOutput;
        source: vlcPlayer;
        anchors.fill: parent;
    }
    MouseArea {
        anchors.fill: videoOutput;
        onClicked: vlcPlayer.togglePause();
    }
}
