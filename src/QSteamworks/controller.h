#pragma once

#include <QObject>

#include "steam/steam_api.h"

namespace QSteamworks {
class Controller {
  Q_GADGET
  Q_PROPERTY(unsigned long long handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString image READ image CONSTANT)

public:
  Controller();
  Controller(InputHandle_t, const QString &, const QString &);

  unsigned long long handle() const;

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

Q_DECLARE_METATYPE(QSteamworks::Controller)
