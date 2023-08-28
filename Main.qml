import QtQuick
import QtQuick.Window


Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("AuS")
    color: "black"

    property string currentTrackInfo: ""

    Image {
        id: albumArt
        width: 640
        height: 640
        // anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    Connections {
        target: spotifyAPI
        function onTrackInfoReceived(trackInfo) {
            currentTrackInfo = trackInfo
        }
        function onAlbumCoverReceived(albumCoverUrl) {
            albumArt.source = albumCoverUrl
        }
    }


    Text {
        id: trackInfoText
        color: "white"
        font.family: "Roboto"
        font.pointSize: 24
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: albumArt.bottom
        anchors.topMargin: 20
        // anchors.centerIn: parent
        text: currentTrackInfo
    }

}

