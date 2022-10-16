#pragma once

#include <QObject>

class SteamUtilsBridge : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool isOnDeck READ isOnDeck NOTIFY isOnDeckChanged)
  Q_PROPERTY(bool isBigPicture READ isBigPicture NOTIFY isBigPictureChanged)

public:
  explicit SteamUtilsBridge(QObject *parent = nullptr);

  bool isOnDeck() const;

  bool isBigPicture() const;

signals:

  void isOnDeckChanged();
  void isBigPictureChanged();
};
