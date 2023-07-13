#include "cachethumbnailimageprovider.h"

#include <algorithm>

#include <QFutureWatcher>
#include <QQuickImageResponse>
#include <QtConcurrent/QtConcurrent>

#include "abstractcache.h"
#include "qfilesystemwatcher.h"

static const auto ThumbnailSizebase = 125;

class AsyncImageResponse : public QQuickImageResponse {
public:
  AsyncImageResponse(const QString &id, const QSize &requestedSize, AbstractCache *cache)
      : m_id(id), m_requestedSize(requestedSize) {
    auto watcher = new QFutureWatcher<void>(this);

    connect(watcher, &QFutureWatcher<void>::finished, this, &AsyncImageResponse::finished);
    connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFileSystemWatcher::deleteLater);

    auto f = QtConcurrent::run([id, requestedSize, this, cache]() {
      if (requestedSize.isEmpty() || cache == nullptr) {
        m_image = fetchImage().first;
        return;
      }

      auto data = cache->withCache(imageCacheId(), [this]() {
        auto pair = fetchImage();
        m_image = pair.first;
        auto skipCache = pair.second;

        QByteArray ba;

        if (!m_image.isNull() && !skipCache) {
          QBuffer buffer(&ba);
          buffer.open(QIODevice::WriteOnly);
          Q_ASSERT(m_image.save(&buffer, "png"));
        }

        return ba;
      });

      if (!data.isEmpty() && !data.isNull()) {
        Q_ASSERT(m_image.loadFromData(data));
      }
    });

    watcher->setFuture(f);
  }

  QQuickTextureFactory *textureFactory() const override {
    return QQuickTextureFactory::textureFactoryForImage(m_image);
  }

private:
  QString m_id;
  QSize m_requestedSize;
  QImage m_image;

  QString imageCacheId() const {
    auto size = findBestSize();
    return QString("%1//%2x%3").arg(m_id).arg(size.width()).arg(size.height());
  }

  QPair<QImage, bool> fetchImage() const {
    QImage image(m_id); // TODO: add support for network sources
    auto skipCache = false;

    auto size = findBestSize();

    if (image.width() < size.width() || image.height() < size.height()) {
      size = image.size();
    }

    if (!image.isNull() && !size.isNull()) {
      image = image.scaled(size, Qt::KeepAspectRatio);
    }
    return QPair{image, skipCache};
  }

  QSize findBestSize() const {
    if (m_requestedSize.isNull()) {
      return m_requestedSize;
    }
    auto bestSize = ThumbnailSizebase;

    auto requestedSize = std::max(m_requestedSize.width(), m_requestedSize.height());

    while (bestSize < requestedSize) {
      bestSize *= 2;
    }

    return QSize(bestSize, bestSize);
  }
};

QQuickImageResponse *CacheThumbnailImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
  return new AsyncImageResponse(id, requestedSize, m_cache);
}

AbstractCache *CacheThumbnailImageProvider::cache() const { return m_cache; }

void CacheThumbnailImageProvider::setCache(AbstractCache *newCache) { m_cache = newCache; }
