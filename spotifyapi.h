#ifndef SPOTIFYAPI_H
#define SPOTIFYAPI_H

#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QTimer>
#include <QString>

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
    void setupOAuth2(const QString&);
    void setupConnections();
    void startInitialPlayback();

private slots:
    void onGranted();
    void onError(const QString & error1, const QString & error2, const QUrl & errorUrl);

public slots:
    void onAuthenticated() {
        timer->start(1000);
    }
    void togglePlayback();

signals:
    void authenticated();
    void trackInfoReceived(const QString &trackName, const QString &artistName, const QString &albumName);
    void albumCoverReceived(const QString &albumCoverUrl);
    void musicToggled(const bool on);

};

#endif // SPOTIFYAPI_H
