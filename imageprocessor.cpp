#include "imageprocessor.h"
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QBitmap>

ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ImageProcessor::onDownloadFinished);
}

void ImageProcessor::onNewUrl(const QString &url) {
    fetchImage(url);
}
void ImageProcessor::fetchImage(const QString &url)
{
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void ImageProcessor::onDownloadFinished(QNetworkReply *reply)
{
    QPixmap pixmap;
    if (reply->error() == QNetworkReply::NoError)
    {
        pixmap.loadFromData(reply->readAll());
        pixmap = roundImageCorners(pixmap);
    }

    if (pixmap.isNull()) {
        qDebug() << "Pixmap is null";
    }

    qDebug() << "Pre save";
    QString tempPath = "/tmp/image.png";
    qDebug() << pixmap.save(tempPath, "PNG");
    qDebug() << "Post save";

    //TODO: convert pixmap to image and emit ehre
    emit imageReady(QUrl::fromLocalFile(tempPath).toString());

}

QPixmap ImageProcessor::roundImageCorners(const QPixmap &image)
{
    QPixmap roundedImage(image.size());
    roundedImage.fill(Qt::transparent);

    QPainter painter(&roundedImage);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(image.rect(), 50, 50); // Adjust the corner radius here

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, image);

    return roundedImage;
}
