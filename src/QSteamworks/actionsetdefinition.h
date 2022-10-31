#pragma once

#include <QObject>

#include "actiondefinition.h"

namespace QSteamworks {

class ActionSetDefinition {
  Q_GADGET

public:
  ActionSetDefinition();
  ActionSetDefinition(const QString &name, const QList<ActionDefinition> &actions);

  const QList<ActionDefinition> &actions() const;

  const QString &name() const;

private:
  QString m_name;
  QList<ActionDefinition> m_actions;
};
} // namespace QSteamworks
