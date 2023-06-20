#pragma once

#include "qobjectdefs.h"
#include <QObject>
#include <stdexcept>

#include <exception>
#include <stdexcept>

namespace QSteamworks {

class QSteamAPI : public QObject {
  Q_OBJECT
public:
  explicit QSteamAPI(QObject *parent = nullptr);
  virtual ~QSteamAPI();

  Q_INVOKABLE
  void runCallbacks() const;
  bool RestartAppIfNecessary() const;
};
} // namespace QSteamworks
