import QtQuick 2.1
import QmlVlc 0.1
import QtQuick.Layouts 1.1

Rectangle {
    RowLayout{
        anchors.fill: parent;
        VlcVideoSurface {
            source: vlcPlayer;
            fillMode: VlcVideoSurface.Stretch;
            Layout.fillWidth: true;
            Layout.fillHeight: true;
        }
        VlcVideoSurface {
            source: vlcPlayer;
            fillMode: VlcVideoSurface.PreserveAspectFit;
            Layout.fillWidth: true;
            Layout.fillHeight: true;
        }
        VlcVideoSurface {
            source: vlcPlayer;
            fillMode: VlcVideoSurface.PreserveAspectCrop;
            Layout.fillWidth: true;
            Layout.fillHeight: true;
        }
    }
    MouseArea {
        anchors.fill: parent;
        onClicked: vlcPlayer.togglePause();
    }
}
