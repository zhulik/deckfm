#pragma once

#include <QObject>

namespace QSteamworks {

class SteamUtils : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool isSteamRunningOnSteamDeck READ isSteamRunningOnSteamDeck CONSTANT)
  Q_PROPERTY(bool isSteamInBigPictureMode READ isSteamInBigPictureMode CONSTANT)

public:
  explicit SteamUtils(QObject *parent = nullptr);

  bool isSteamRunningOnSteamDeck() const;
  bool isSteamInBigPictureMode() const;
};
}; // namespace QSteamworks
