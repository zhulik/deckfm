#pragma once

#include <QObject>

namespace QSteamworks {

class QSteamAPI;

class QSteamUtils : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool isSteamRunningOnSteamDeck READ isSteamRunningOnSteamDeck CONSTANT)
  Q_PROPERTY(bool isSteamInBigPictureMode READ isSteamInBigPictureMode CONSTANT)

public:
  explicit QSteamUtils(QSteamworks::QSteamAPI *parent = nullptr);

  bool isSteamRunningOnSteamDeck() const;
  bool isSteamInBigPictureMode() const;
};
}; // namespace QSteamworks
