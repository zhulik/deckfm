#pragma once

#include <QObject>
#include <functional>

class AbstractCache : public QObject {
  Q_OBJECT
public:
  explicit AbstractCache(QObject *parent = nullptr);

  virtual QByteArray get(const QString &) const = 0;
  virtual void set(const QString &, const QByteArray &) = 0;
  virtual bool exists(const QString &) const = 0;

  QByteArray withCache(const QString &, std::function<QByteArray()>);

signals:
  void cacheUpdated();
};
