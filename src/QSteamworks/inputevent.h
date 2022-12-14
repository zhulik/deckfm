#pragma once

#include <QObject>

#include "action.h"
#include "controller.h"

namespace QSteamworks {

class InputEvent {
  Q_GADGET
  Q_PROPERTY(QString type READ type CONSTANT)

  Q_PROPERTY(QSteamworks::Controller controller READ controller CONSTANT)
  Q_PROPERTY(QSteamworks::Action action READ action CONSTANT)

  Q_PROPERTY(bool digitalState READ digitalState CONSTANT)
  Q_PROPERTY(float analogX READ analogX CONSTANT)
  Q_PROPERTY(float analogY READ analogY CONSTANT)

public:
  InputEvent() {}

  InputEvent(const QString &type, const Controller &controller, const Action &action, bool digitalState, float analogX,
             float analogY);

  const QString &type() const;
  Controller controller() const;

  Action action() const;

  bool digitalState() const;
  float analogX() const;
  float analogY() const;

private:
  QString m_type;
  Controller m_controller;
  Action m_action;
  bool m_digitalState = false;
  float m_analogX = 0;
  float m_analogY = 0;
};
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::InputEvent)
