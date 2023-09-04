#include "imageprocessor.h"
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QBitmap>

bool ImageProcessor::grayscale = false;

ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{
    qDebug() << "set grayscale to false";
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
    QImage roundedImage = pixmap.toImage();
    if(grayscale)
    {
        roundedImage = roundedImage.convertToFormat(QImage::Format_Grayscale8);
    }
    QString tempPath = "/tmp/image.png";
    //qDebug() << pixmap.save(tempPath, "PNG");
    roundedImage.save(tempPath, "PNG");

    QString imagePath = QUrl::fromLocalFile(tempPath).toString();
    QString dynamicImagePath = imagePath + "?" + QString::number(QDateTime::currentMSecsSinceEpoch());
    emit imageReady(dynamicImagePath);
}

void ImageProcessor::onMusicToggled(const bool on)
{
    grayscale = !on;
    qDebug() << "Set grayscale to " << grayscale;
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
