#ifndef SPOTIFYAPI_H
#define SPOTIFYAPI_H

#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QTimer>

class SpotifyAPI : public QObject
{
    Q_OBJECT
public:
    explicit SpotifyAPI(QObject *parent = nullptr);
    void authenticate();
    void getCurrentPlayingTrack();

private:
    QOAuth2AuthorizationCodeFlow* m_oauth2;
    QTimer* timer = new QTimer(this);

private slots:
    void onGranted();
    void onError(const QString & error1, const QString & error2, const QUrl & errorUrl);

public slots:
    void onAuthenticated() {
        timer->start(1000);
    }

signals:
    void authenticated();
    void trackInfoReceived(const QString &trackInfo);
    void albumCoverReceived(const QString &albumCoverUrl);

};

#endif // SPOTIFYAPI_H
