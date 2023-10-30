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

private:
    QNetworkAccessManager *networkManager;
    static bool grayscale;
    void setupConnections();
    QPixmap roundImageCorners(const QPixmap &image);
    QString saveImage(const QImage &img);
    void startImageDownload(const QString &url);

signals:
    void imageReady(const QUrl &url);

public slots:
    void onNewUrl(const QString &url);
    void onMusicToggled(const bool on);

private slots:
    void onDownloadFinished(QNetworkReply *reply);

};

#endif // IMAGEPROCESSOR_H
