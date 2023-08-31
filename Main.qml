import QtQuick
import QtQuick.Window


Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("AuS")
    color: "black"

    property string currentTrackName: ""
    property string currentArtistName: ""
    property string currentAlbumName: ""


    Rectangle {
        id: albumArtContainer
        width: 640
        height: 640
        color: "grey"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        radius: 50
        clip: true

        Image {
            visible: false
            id: albumArt
            anchors.fill: parent
        }
    }


    Connections {
        target: spotifyAPI
        function onTrackInfoReceived(trackName, artistName, albumName) {
            currentTrackName = trackName
            currentArtistName = artistName
            currentAlbumName = albumName
        }
        function onAlbumCoverReceived(albumCoverUrl) {
            albumArt.source = albumCoverUrl
        }
    }


    Text {
        id: trackName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 24
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: albumArtContainer.bottom
        anchors.topMargin: 10
        // anchors.centerIn: parent
        text: currentTrackName
    }

    Text {
        id: artistName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 20
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: trackName.bottom
        anchors.topMargin: 5
        // anchors.centerIn: parent
        text: currentArtistName
    }

    Text {
        id: albumName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 20
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: artistName.bottom
        anchors.topMargin: 5
        // anchors.centerIn: parent
        text: currentAlbumName
    }

}

