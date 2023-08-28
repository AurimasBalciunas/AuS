#include "spotifyapi.h"

#include <QOAuthHttpServerReplyHandler>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcessEnvironment>

SpotifyAPI::SpotifyAPI(QObject *parent)
    : QObject{parent}
{
    m_oauth2 = new QOAuth2AuthorizationCodeFlow(this);

    m_oauth2->setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    m_oauth2->setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    m_oauth2->setClientIdentifier("b90c5291239b412a86d07ac6bd8190af");
    QString sharedKey = QProcessEnvironment::systemEnvironment().value("SPOTIFY_CLIENT_SECRET", "");
    qDebug() << "Shared key" << sharedKey;
    m_oauth2->setClientIdentifierSharedKey(sharedKey);
    m_oauth2->setScope("user-read-private user-read-playback-state user-modify-playback-state");

    auto replyHandler = new QOAuthHttpServerReplyHandler(3000); // or another port
    m_oauth2->setReplyHandler(replyHandler);


    // Connect signals for authentication
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::granted, this, &SpotifyAPI::onGranted);
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::error, this, &SpotifyAPI::onError);

    // Set up a post-authentication timer to call getCurrentlyPlayingTrack
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SpotifyAPI::getCurrentPlayingTrack);


    SpotifyAPI::authenticate();
}

void SpotifyAPI::authenticate()
{
    m_oauth2->grant();
}

void SpotifyAPI::onGranted()
{
    QString bearerToken = m_oauth2->token();
    printf("Bearer token is %s", bearerToken.toUtf8().constData());
    qDebug() << "\nBearer token is :" << bearerToken << Qt::endl;
    emit authenticated();
    timer->start(1000);
}

void SpotifyAPI::onError(const QString & error1, const QString & error2, const QUrl & errorUrl)
{
    //TODO: Change to UI-level warning and potential log rather than closing out fully
    printf("\nHad an error\n");
    exit(1);
}

void SpotifyAPI::getCurrentPlayingTrack()
{
    QUrl url("https://api.spotify.com/v1/me/player/currently-playing");
    auto reply = m_oauth2->get(url);
    qDebug("getting track info");
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Network error" << reply->errorString();
            return;
        }

        auto jsonData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject obj = doc.object();
        QJsonObject itemObject = obj["item"].toObject();

        QString trackName = itemObject["name"].toString();

        QJsonArray artistsArray = itemObject["artists"].toArray();
        QJsonObject artistObject = artistsArray.first().toObject();
        QString artistName = artistObject["name"].toString();

        // Process JSON data and extract track info
        qDebug("got track info");
        qDebug() << "Track Name: " << trackName;
        qDebug() << "Artist Name: " << artistName;
        emit trackInfoReceived(trackName + " by " + artistName);

        QString albumCoverUrl = itemObject["album"].toObject()["images"].toArray()[0].toObject()["url"].toString();
        emit albumCoverReceived(albumCoverUrl);
    });
}
