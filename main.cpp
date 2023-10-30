#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QOpenGLContext>
#include "imageprocessor.h"

#include "spotifyapi.h"

int main(int argc, char *argv[])
{

    // qt stuff
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // image processor object setup
    ImageProcessor imageProcessor;
    engine.rootContext()->setContextProperty("imageProcessor", &imageProcessor);

    // spotify api object setup
    SpotifyAPI spotifyAPI;
    engine.rootContext()->setContextProperty("spotifyAPI", &spotifyAPI); // Make it available to QML

    // qt stuff
    const QUrl url(u"qrc:/aus/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // new album cover URL -> image processor informed (round corners and send back)
    QObject::connect(&spotifyAPI, &SpotifyAPI::albumCoverReceived, &imageProcessor, &ImageProcessor::onNewUrl);

    // music toggled -> image processor informed (update grayscale)
    QObject::connect(&spotifyAPI, &SpotifyAPI::musicToggled, &imageProcessor, &ImageProcessor::onMusicToggled);

    engine.load(url);
    return app.exec();
}
