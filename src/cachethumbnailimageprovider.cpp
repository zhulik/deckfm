#include "cachethumbnailimageprovider.h"

#include <QFutureWatcher>
#include <QQuickImageResponse>
#include <QtConcurrent/QtConcurrent>

#include "abstractcache.h"

class AsyncImageResponse : public QQuickImageResponse {
public:
  AsyncImageResponse(const QString &id, const QSize &requestedSize, AbstractCache *cache) {
    auto watcher = new QFutureWatcher<QImage>();

    auto f = QtConcurrent::run([id, requestedSize, this, cache]() {
      QImage result;

      if (requestedSize.isEmpty()) {
        return result;
      }

      if (cache != nullptr) {
        auto data = cache->withCache(imageCacheId(id, requestedSize), [id, requestedSize]() {
          QByteArray ba;
          QImage image(id); // TODO: add support for network sources
          if (image.isNull()) {
            return ba;
          }
          if (!image.isNull()) {
            image = image.scaled(requestedSize, Qt::KeepAspectRatio);
          }
          QBuffer buffer(&ba);
          buffer.open(QIODevice::WriteOnly);
          Q_ASSERT(image.save(&buffer, "png"));
          qDebug() << ba.size();
          return ba;
        });

        result.loadFromData(data);
      } else {
        QImage image(id); // TODO: add support for network sources
        if (!image.isNull()) {
          result = image.scaled(requestedSize, Qt::KeepAspectRatio);
        }
      }
      return result;
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

private:
  QImage m_image;

  QString imageCacheId(const QString &id, const QSize &size) const {
    return QString("%1//%2x%3").arg(id).arg(size.width()).arg(size.height());
  }
};

QQuickImageResponse *CacheThumbnailImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
  return new AsyncImageResponse(id, requestedSize, m_cache);
}

AbstractCache *CacheThumbnailImageProvider::cache() const { return m_cache; }

void CacheThumbnailImageProvider::setCache(AbstractCache *newCache) { m_cache = newCache; }
