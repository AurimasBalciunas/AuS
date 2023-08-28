#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "spotifyapi.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    SpotifyAPI spotifyAPI; // Creating Spotify API class
    engine.rootContext()->setContextProperty("spotifyAPI", &spotifyAPI); // Make it available to QML

    const QUrl url(u"qrc:/aus/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
