/*****************************************************************************
* Copyright (c) 2014 Sergey Radionov <rsatom_gmail.com>
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
import QtQuick.Layouts 1.0
import QmlVlc 0.1

Rectangle {
    color: bgcolor;
    VlcVideoSurface {
        source: vlcPlayer;
        anchors.top: parent.top;
        anchors.topMargin: 10;
        anchors.left: parent.left;
        anchors.leftMargin: anchors.topMargin;
        width: parent.width / 2 - anchors.leftMargin * 2;
        height: parent.height / 2 - anchors.topMargin * 2;
    }
    VlcVideoSurface {
        source: vlcPlayer;
        anchors.top: parent.top;
        anchors.topMargin: 10;
        anchors.right: parent.right;
        anchors.rightMargin: anchors.topMargin;
        width: parent.width / 2 - anchors.rightMargin * 2;
        height: parent.height / 2 - anchors.topMargin * 2;
    }
    VlcVideoSurface {
        source: vlcPlayer;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 10;
        anchors.left: parent.left;
        anchors.leftMargin: anchors.bottomMargin;
        width: parent.width / 2 - anchors.leftMargin * 2;
        height: parent.height / 2 - anchors.bottomMargin * 2;
    }
    VlcVideoSurface {
        source: vlcPlayer;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 10;
        anchors.right: parent.right;
        anchors.rightMargin: anchors.bottomMargin;
        width: parent.width / 2 - anchors.rightMargin * 2;
        height: parent.height / 2 - anchors.bottomMargin * 2;
    }
    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        RowLayout {
            id: toolbar
            opacity: .55
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.containsMouse ? spacing : -height
            anchors.leftMargin: spacing * 1.5
            anchors.rightMargin: spacing * 1.5
            Behavior on anchors.bottomMargin { PropertyAnimation { duration: 250} }
            Rectangle {
                height: 24
                width: height
                radius: width * 0.25
                color: 'black'
                border.width: 1
                border.color: 'white'
                Image {
                    source: vlcPlayer.playing ? "pause.png" : "play.png"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: vlcPlayer.togglePause()
                }
            }
            Rectangle {
                Layout.fillWidth: true
                height: 10
                color: 'transparent'
                border.width: 1
                border.color: 'white'
                anchors.verticalCenter: parent.verticalCenter
                Rectangle {
                    width: (parent.width - anchors.leftMargin - anchors.rightMargin) * vlcPlayer.position
                    color: 'white'
                    anchors.margins: 2
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                }
            }
        }
    }
}
