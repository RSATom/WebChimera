import QtQuick 2.1
import QmlVlc 0.1

Rectangle {
    color: bgcolor
    VlcVideoSurface {
        id: videoOutput;
        source: player;
        anchors.fill: parent;
    }
    Text{
        id: debugText;
        anchors.fill: parent;
        text: "-----";
        color: "red";
    }
    MouseArea {
        anchors.fill: videoOutput;
        onClicked: player.togglePause();
    }
    Component.onCompleted: {
        player.aceStream.showUserDataDialog.connect(
            function() {
                player.aceStream.setUserData( AceStreamObject.Male, AceStreamObject.AgeUnder13 );
            }
        );
        player.onLivePosChanged.connect(
            function() {
                debugText.text = "onLivePosChanged";
            }
        );
        player.aceStream.loadTorrent("http://content.torrent-tv.ru/cdn/223_all.acelive");
    }
}
