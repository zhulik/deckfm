#pragma once

#include <QQuickAsyncImageProvider>
#include <QThreadPool>

class AbstractCache;

class CacheThumbnailImageProvider : public QQuickAsyncImageProvider {
public:
  virtual QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize);

  AbstractCache *cache() const;
  void setCache(AbstractCache *newCache);

private:
  AbstractCache *m_cache = nullptr;
};
