#pragma once

#include <QObject>

#include "steam/steam_api.h"

namespace QSteamworks {
class Controller : public QObject {
  Q_OBJECT
  Q_PROPERTY(InputHandle_t handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString image READ image CONSTANT)

public:
  Controller(InputHandle_t, const QString &, QObject *parent = nullptr);

  InputHandle_t handle() const;

  const QString &name() const;

  const QString &image() const;

  bool operator==(const Controller &) const;

private:
  InputHandle_t m_handle = 0;
  QString m_name;
  QString m_image;
};

inline uint qHash(const QSteamworks::Controller &key) { return ::qHash(key.handle()); }

} // namespace QSteamworks
