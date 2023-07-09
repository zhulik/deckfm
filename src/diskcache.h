#pragma once

#include "abstractcache.h"
#include <QDir>
#include <QMap>
#include <QObject>

class DiskCache : public AbstractCache {
  Q_OBJECT
public:
  explicit DiskCache(QObject *parent = nullptr);

  // AbstractCache interface
  QByteArray get(const QString &) const;
  void set(const QString &, const QByteArray &);
  bool exists(const QString &) const;

private:
  QDir m_root;

  QFileInfo keyToPath(const QString &id) const;
};
