import QtQuick 2.1
import QmlVlc 0.1
import QtWebEngine 1.2
import QtWebChannel 1.0

Rectangle {
    color: bgcolor
    VlcVideoSurface {
        id: videoOutput;
        source: player;
        anchors.fill: parent;
    }
    WebEngineView {
        id: webView;
        url: "http://rsatom.github.io/WebChimera/demos/basic/webengine.index.htm";
        anchors.fill: parent;
        backgroundColor: "transparent";
    }
    Component.onCompleted: {
        webView.webChannel.registerObject( 'player', player );
    }
}
