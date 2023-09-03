import QtQuick
import QtQuick.Window
import QtQuick.Controls
Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("AuS")
    color: "black"

    property string currentTrackName: ""
    property string currentArtistName: ""
    property string currentAlbumName: ""



    Image {
        width: 640
        height: 640
        visible: true
        id: albumArt
        anchors.centerIn: parent
        source: "file:///path/to/placeholder.png" //TODO: Set this placeholder
    }

    //    Button {
    //        text: "Play"
    //        anchors.centerIn: albumArt
    //        onClicked: {
    //            console.log("Button clicked")
    //        }
    //    }

    Text {
        id: trackName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 24
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: albumArt.bottom
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

    //SpotifyAPI connections
    Connections {
        target: spotifyAPI
        function onTrackInfoReceived(trackName, artistName, albumName) {
            currentTrackName = trackName
            currentArtistName = artistName
            currentAlbumName = albumName
        }
        function onAlbumCoverReceived(albumCoverUrl) {
            //albumArt.source = ""
            //albumArt.source = albumCoverUrl
            //console.log(albumCoverUrl)
            console.log("album cover received function")
        }
    }

    // ImageProcessor Connections
    Connections {
        target: imageProcessor
        function onImageReady(imagePath) {
            albumArt.source = ""
            albumArt.source = imagePath

        }
    }

}

