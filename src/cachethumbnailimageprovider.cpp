#include "cachethumbnailimageprovider.h"

#include <QFutureWatcher>
#include <QQuickImageResponse>
#include <QtConcurrent/QtConcurrent>

#include "qfuturewatcher.h"

class AsyncImageResponse : public QQuickImageResponse {
public:
  AsyncImageResponse(const QString &id, const QSize &requestedSize) {
    auto watcher = new QFutureWatcher<QImage>();
    auto f = QtConcurrent::run([id, requestedSize, this]() {
      if (requestedSize.isEmpty()) {
        return QImage();
      }
      QImage image(id); // TODO: add support for network sources
      return image.scaled(requestedSize, Qt::KeepAspectRatio);
    });

    connect(watcher, &QFutureWatcher<QImage>::finished, watcher, [this, f, watcher]() {
      m_image = f.result();
      emit finished();
      watcher->deleteLater();
    });

    watcher->setFuture(f);
  }

  QQuickTextureFactory *textureFactory() const override {
    return QQuickTextureFactory::textureFactoryForImage(m_image);
  }

  QImage m_image;
};

QQuickImageResponse *CacheThumbnailImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
  return new AsyncImageResponse(id, requestedSize);
}
