/*****************************************************************************
* Copyright © 2014 Sergey Radionov <rsatom_gmail.com>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
*****************************************************************************/

import QtQuick 2.1
import QmlVlc 0.1

Rectangle {
    id: bg
    color: bgcolor
    VlcVideoSurface {
        id: videoOutput
        source: vlcPlayer
        anchors.fill: parent
        MouseArea {
            id: bgArea
            hoverEnabled: true
            anchors.fill: parent
            Item {
                id: toolbar
                height: 35
                opacity: .6
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: bgArea.containsMouse ? parent.height * .03 : -height
                Behavior on anchors.bottomMargin { PropertyAnimation { duration: 250} }
                Rectangle {
                    height: 30
                    width: height
                    radius: width * 0.25
                    color: 'black'
                    border.width: 1
                    border.color: 'white'
                    anchors.centerIn: parent
                    Image {
                        source: vlcPlayer.playing ? "pause.png" : "play.png"
                        anchors.centerIn: parent
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: vlcPlayer.togglePause()
                    }
                }
            }
        }
    }
}
