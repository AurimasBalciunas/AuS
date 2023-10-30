#include "imageprocessor.h"
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QBitmap>

bool ImageProcessor::grayscale = false;

// ctor
ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ImageProcessor::onDownloadFinished);
}

/////////////////////////////////////
// Public Slots
/////////////////////////////////////

// Triggered upon new album art URL
void ImageProcessor::onNewUrl(const QString &url) {
    startImageDownload(url);
}

// TODO: change to onMusicPaused and onMusicStarted
void ImageProcessor::onMusicToggled(const bool on)
{
    grayscale = !on;
    qDebug() << "Set grayscale to " << grayscale;
}


/////////////////////////////////////
// Private Slots
/////////////////////////////////////

//TODO: split out into helper fxns
// slot to process image and update UI upon new album art download
void ImageProcessor::onDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error in album art download";
        return;
    }

    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    pixmap = roundImageCorners(pixmap);
    QImage roundedImage = pixmap.toImage();
    
    if(grayscale)
    {
        roundedImage = roundedImage.convertToFormat(QImage::Format_Grayscale8);
    }
    
    QString path = saveImage(roundedImage);
    emit imageReady(path);

}


/////////////////////////////////////
// Helpers
/////////////////////////////////////

// fxn to round image corners using QPainter
QPixmap ImageProcessor::roundImageCorners(const QPixmap &image)
{
    QPixmap roundedImage(image.size());
    roundedImage.fill(Qt::transparent);

    QPainter painter(&roundedImage);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(image.rect(), 50, 50);

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, image);

    return roundedImage;
}

// fxn to save image to file system
QString ImageProcessor::saveImage(const QImage &img)
{
    QString tempPath = "/tmp/image.png";
    img.save(tempPath, "PNG");
    QString imagePath = QUrl::fromLocalFile(tempPath).toString();
    
    // A little hacky:
    // everything after ? is ignored by the system, but emitted image path 
    // appears to change so the GUI refreshes the image
    QString dynamicImagePath = imagePath + "?" + QString::number(QDateTime::currentMSecsSinceEpoch());
    return dynamicImagePath;
}

// fxn to launch NetworkManager image download
void ImageProcessor::startImageDownload(const QString &url)
{
    networkManager->get(QNetworkRequest(QUrl(url)));
}

