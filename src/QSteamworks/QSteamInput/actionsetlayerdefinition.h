#pragma once

#include <QObject>

#include "actiondefinition.h"

namespace QSteamworks {

using namespace QSteamInput;

class ActionSetLayerDefinition {
  Q_GADGET
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionDefinition> actions READ actions CONSTANT)

public:
  ActionSetLayerDefinition(){};
  ActionSetLayerDefinition(const QString &name, const QList<ActionDefinition> &actions);

  const QString &name() const;

  const QList<ActionDefinition> &actions() const;

private:
  QString m_name;
  QList<ActionDefinition> m_actions;
};
} // namespace QSteamworks

Q_DECLARE_METATYPE(QSteamworks::ActionSetLayerDefinition)
