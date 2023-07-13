#include "diskcache.h"

#include <QCryptographicHash>
#include <QStandardPaths>

DiskCache::DiskCache(QObject *parent) : AbstractCache{parent} {
  m_root = QDir(QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0]);
  m_root.mkpath("thumbnails");
  m_root.cd("thumbnails");

  Q_ASSERT(m_root.exists());
}

QByteArray DiskCache::get(const QString &key) const {
  auto info = keyToPath(key);
  if (m_root.exists(info.filePath())) {
    QFile f(m_root.absoluteFilePath(info.filePath()));
    f.open(QIODevice::ReadOnly);
    auto data = f.readAll();
    f.close();
    return data;
  } else {
    return QByteArray();
  }
}

void DiskCache::set(const QString &key, const QByteArray &data) {
  if (data.isEmpty()) {
    return;
  }
  auto info = keyToPath(key);
  Q_ASSERT(m_root.mkpath(info.path()));

  QFile file(m_root.absoluteFilePath(info.filePath()));

  Q_ASSERT(file.open(QIODevice::WriteOnly));

  file.write(data);
  file.close();
  emit cacheUpdated();
}

bool DiskCache::exists(const QString &key) const { return m_root.exists(keyToPath(key).filePath()); }

QFileInfo DiskCache::keyToPath(const QString &key) const {
  auto parts = key.split("//");
  QFileInfo info(parts[0]);
  Q_ASSERT(info.exists());

  auto size = parts[1].split("x");
  Q_ASSERT(size.length() == 2);

  QCryptographicHash hash(QCryptographicHash::Sha256);
  hash.addData(info.absoluteFilePath().toLocal8Bit());

  auto hashStr = hash.result().toHex();

  return QFileInfo(QString("%1/%2/%3/%4_%5x%6.%7")
                       .arg(hashStr[0])
                       .arg(hashStr[1])
                       .arg(hashStr[2])
                       .arg(QString(hashStr))
                       .arg(size[0])
                       .arg(size[1])
                       .arg(info.suffix()));
}
