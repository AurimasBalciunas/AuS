import QtQuick
import QtQuick.Window
import QtQuick.Controls
Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("AuS")
    color: "black"
    visibility: Window.FullScreen

    property string currentTrackName: ""
    property string currentArtistName: ""
    property string currentAlbumName: ""


    Image {
        width: 640
        height: 640
        visible: true
        id: albumArt
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / 8
        source: "file:///path/to/placeholder.png" //TODO: Set this placeholder

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: {
                console.log("Image clicked!")
                spotifyAPI.togglePlayback();
            }
        }

    }

    Text {
        id: trackName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 40
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: albumArt.bottom
        anchors.topMargin: parent.height/16
        text: currentTrackName
    }

    Text {
        id: artistName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 30
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: trackName.bottom
        anchors.topMargin: 10
        text: currentArtistName
    }

    Text {
        id: albumName
        color: "white"
        font.family: "Roboto"
        font.pointSize: 30
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: artistName.bottom
        anchors.topMargin: 10
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
