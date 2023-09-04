#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class ImageProcessor : public QObject
{
    Q_OBJECT

public:
    explicit ImageProcessor(QObject *parent = nullptr);
    Q_INVOKABLE void fetchImage(const QString &url);

signals:
    void imageReady(const QUrl &url);

public slots:
    void onNewUrl(const QString &url);
    void onMusicToggled(const bool on);


private slots:
    void onDownloadFinished(QNetworkReply *reply);

private:
    static bool grayscale;
    QPixmap roundImageCorners(const QPixmap &image);
    QNetworkAccessManager *networkManager;
    void setupConnections();

};

#endif // IMAGEPROCESSOR_H
