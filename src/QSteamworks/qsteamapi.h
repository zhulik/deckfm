#pragma once

#include "qobjectdefs.h"
#include <QObject>
#include <stdexcept>

#include <exception>
#include <stdexcept>

namespace QSteamworks {

class QSteamInput;

class QSteamAPI : public QObject {
  Q_OBJECT
  Q_PROPERTY(QSteamInput *steamInput READ steamInput CONSTANT)
public:
  explicit QSteamAPI(QObject *parent = nullptr);
  virtual ~QSteamAPI();

  Q_INVOKABLE
  void runCallbacks() const;

  QSteamInput *steamInput();

signals:

private:
  QSteamInput *m_steamInput = nullptr;
};
} // namespace QSteamworks
