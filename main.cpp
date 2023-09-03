#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QOpenGLContext>
#include "imageprocessor.h"

#include "spotifyapi.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;

    ImageProcessor imageProcessor;
    engine.rootContext()->setContextProperty("imageProcessor", &imageProcessor);

    SpotifyAPI spotifyAPI;
    engine.rootContext()->setContextProperty("spotifyAPI", &spotifyAPI); // Make it available to QML

    const QUrl url(u"qrc:/aus/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    QObject::connect(&spotifyAPI, &SpotifyAPI::albumCoverReceived, &imageProcessor, &ImageProcessor::onNewUrl);
    engine.load(url);


    return app.exec();
}
