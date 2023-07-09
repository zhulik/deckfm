#pragma once

#include <QQuickAsyncImageProvider>
#include <QThreadPool>

class CacheThumbnailImageProvider : public QQuickAsyncImageProvider {
public:
  virtual QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize);
};
