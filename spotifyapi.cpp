#include "spotifyapi.h"

#include <QOAuthHttpServerReplyHandler>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcessEnvironment>

constexpr auto AUTHORIZATION_URL = "https://accounts.spotify.com/authorize";
constexpr auto ACCESS_TOKEN_URL = "https://accounts.spotify.com/api/token";
constexpr auto CLIENT_IDENTIFIER = "b90c5291239b412a86d07ac6bd8190af";
constexpr auto SCOPE = "user-read-private user-read-playback-state user-modify-playback-state";
constexpr int TIMER_INTERVAL_MS = 1000;
constexpr int SERVER_PORT = 3000;

// ctor
SpotifyAPI::SpotifyAPI(QObject *parent)
    : QObject{parent}, timer{new QTimer(this)}
{
    const QString sharedKey = QProcessEnvironment::systemEnvironment().value("SPOTIFY_CLIENT_SECRET", "");
    setupOAuth2(sharedKey);
    setupConnections();
    SpotifyAPI::authenticate();
}


/////////////////////////////////////
// Public Slots
/////////////////////////////////////

// slot hit upon authentication completion
void SpotifyAPI::authenticate()
{
    m_oauth2->grant();
}

// slot hit to request playback toggle
void SpotifyAPI::togglePlayback()
{
    volatile bool isPlaying = false;
    QUrl isPlayingUrl("https://api.spotify.com/v1/me/player/");
    auto reply = m_oauth2->get(isPlayingUrl);
    connect(reply, &QNetworkReply::finished, [this, reply, &isPlaying]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Network error" << reply->errorString();
            return;
        }
        auto jsonData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull())
        {
            qDebug() << "TP: No JSON returned by Spotify API";
            //startInitialPlayback();
            return;
        }
        QJsonObject obj = doc.object();
        isPlaying = obj["is_playing"].toBool();
        qDebug() << "Initial isPlaying is " << isPlaying;

        QUrl toggleUrl = QUrl("");
        if(isPlaying)
        {
            qDebug() << "Attempting to pause";
            sendPlaybackCommand(QUrl("https://api.spotify.com/v1/me/player/pause"), false);
        }
        else
        {
            qDebug() << "Attempting to play";
            sendPlaybackCommand(QUrl("https://api.spotify.com/v1/me/player/play"), true);
        }
        reply->deleteLater();
    });
}


/////////////////////////////////////
// Private Slots
/////////////////////////////////////

// slot for authentication. kind of a duplicate
void SpotifyAPI::onGranted()
{
    emit authenticated();
    timer->start(TIMER_INTERVAL_MS);
    qDebug() << m_oauth2->token();
}

// OAuth Error slot
void SpotifyAPI::onError(const QString & error1, const QString & error2, const QUrl & errorUrl)
{
    qDebug() << "OAuth Error";
    exit(1);
}


/////////////////////////////////////
// Helpers
/////////////////////////////////////

// fxn to pull current track
void SpotifyAPI::getCurrentPlayingTrack()
{
    QUrl url("https://api.spotify.com/v1/me/player/currently-playing");
    auto reply = m_oauth2->get(url);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Network error" << reply->errorString();
            return;
        }

        auto jsonData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull())
        {
            qDebug() << "No JSON returned by Spotify API";
            return;
        }

        QJsonObject obj = doc.object();
        if (obj.isEmpty() || !obj.contains("item"))
        {
            qDebug() << "Empty or invalid item object";
            return;
        }

        QJsonObject itemObject = obj["item"].toObject();
        if(!itemObject.contains("artists") || itemObject["artists"].toArray().isEmpty())
        {
            qDebug() << "Response lacks artist information";
            return;
        }
        if(!itemObject.contains("album"))
        {
            qDebug() << "Response lacks album information";
            return;
        }

        QString trackName = itemObject["name"].toString();
        QJsonArray artistsArray = itemObject["artists"].toArray();
        QJsonObject artistObject = artistsArray.first().toObject();
        QString artistName = artistObject["name"].toString();
        QString albumName = itemObject["album"].toObject()["name"].toString();
        emit trackInfoReceived(trackName, artistName, albumName);

        QString albumCoverUrl = itemObject["album"].toObject()["images"].toArray()[0].toObject()["url"].toString();
        emit albumCoverReceived(albumCoverUrl);

        reply->deleteLater();
    });
}

// fxn to set up class connnections
void SpotifyAPI::setupConnections()
{
    // OAuth2 Signals
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::granted, this, &SpotifyAPI::onGranted);
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::error, this, &SpotifyAPI::onError);

    // Timer signal to update track
    connect(timer, &QTimer::timeout, this, &SpotifyAPI::getCurrentPlayingTrack);
}

// fxn to set up OAuth connection w/ Spotify API 
void SpotifyAPI::setupOAuth2(const QString& sharedKey)
{
    m_oauth2 = new QOAuth2AuthorizationCodeFlow(this);
    m_oauth2->setAuthorizationUrl(QUrl(AUTHORIZATION_URL));
    m_oauth2->setAccessTokenUrl(QUrl(ACCESS_TOKEN_URL));
    m_oauth2->setClientIdentifier(CLIENT_IDENTIFIER);
    m_oauth2->setClientIdentifierSharedKey(sharedKey);
    m_oauth2->setScope(SCOPE);

    auto replyHandler = new QOAuthHttpServerReplyHandler(3000);
    m_oauth2->setReplyHandler(replyHandler);
}

//TODO: Either function or API endpoint a little glitchy. Debug and fix
// fxn to wrap playback
void SpotifyAPI::sendPlaybackCommand(const QUrl& commandUrl, bool wasAttemptingToPlay)
{
    auto putReply = m_oauth2->put(commandUrl);
    connect(putReply, &QNetworkReply::finished, [this, putReply, wasAttemptingToPlay]() {
        if (putReply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Network error while attempting playback command:" << putReply->errorString();

            if (wasAttemptingToPlay)
            {
                qDebug() << "Error while attempting to play. Trying to pause instead.";
                sendPlaybackCommand(QUrl("https://api.spotify.com/v1/me/player/pause"), false);
            }
            else
            {
                qDebug() << "Error while attempting to pause. Trying to play instead.";
                sendPlaybackCommand(QUrl("https://api.spotify.com/v1/me/player/play"), true);
            }

            putReply->deleteLater();
            return;
        }
        emit musicToggled(wasAttemptingToPlay);
        SpotifyAPI::getCurrentPlayingTrack();
        putReply->deleteLater();

    });
}

