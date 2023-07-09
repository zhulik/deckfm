#pragma once

#include <QObject>

#include "steam/isteamfriends.h"

namespace QSteamworks {

void registerTypes();

class SteamAPI : public QObject {
  Q_OBJECT

  STEAM_CALLBACK(SteamAPI, onGameOverlayActivated, GameOverlayActivated_t);

public:
  explicit SteamAPI(QObject *parent = nullptr);
  virtual ~SteamAPI();

  Q_INVOKABLE
  void runCallbacks() const;
  bool RestartAppIfNecessary() const;

signals:
  void overlayOpened();
  void overlayClosed();
};
} // namespace QSteamworks
