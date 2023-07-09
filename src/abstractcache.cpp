#include "abstractcache.h"

AbstractCache::AbstractCache(QObject *parent) : QObject{parent} {}

QByteArray AbstractCache::withCache(const QString &key, std::function<QByteArray()> f) {
  if (!exists(key)) {
    set(key, f());
  }

  return get(key);
}
