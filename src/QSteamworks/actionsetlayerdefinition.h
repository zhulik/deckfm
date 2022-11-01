#pragma once

#include <QObject>

#include "actiondefinition.h"

namespace QSteamworks {
class ActionSetLayerDefinition {
  Q_GADGET
public:
  ActionSetLayerDefinition(const QString &name, const QList<ActionDefinition> &actions);

  const QString &name() const;

  const QList<ActionDefinition> &actions() const;

private:
  QString m_name;
  QList<ActionDefinition> m_actions;
};
} // namespace QSteamworks
