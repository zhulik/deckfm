#pragma once

#include "qobjectdefs.h"
#include <QObject>
#include <stdexcept>

#include <exception>
#include <stdexcept>

namespace QSteamworks {

void registerTypes();

class SteamAPI : public QObject {
  Q_OBJECT
public:
  explicit SteamAPI(QObject *parent = nullptr);
  virtual ~SteamAPI();

  Q_INVOKABLE
  void runCallbacks() const;
  bool RestartAppIfNecessary() const;
};
} // namespace QSteamworks
