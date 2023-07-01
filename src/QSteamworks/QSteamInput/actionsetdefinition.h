#pragma once

#include <QObject>

#include "actiondefinition.h"
#include "actionsetlayerdefinition.h"

namespace QSteamworks {
namespace QSteamInput {

class ActionSetDefinition {
  Q_GADGET
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionDefinition> actions READ actions CONSTANT)
  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionSetLayerDefinition> layers READ layers CONSTANT)

public:
  ActionSetDefinition(){};
  ActionSetDefinition(const QString &name, const QList<ActionDefinition> &actions,
                      const QList<ActionSetLayerDefinition> &layers);

  const QList<ActionDefinition> &actions() const;

  const QString &name() const;

  const QList<ActionSetLayerDefinition> &layers() const;

private:
  QString m_name;
  QList<ActionDefinition> m_actions;
  QList<ActionSetLayerDefinition> m_layers;
};
} // namespace QSteamInput
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::QSteamInput::ActionSetDefinition)
