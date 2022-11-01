#pragma once

#include <QObject>

#include "actiondefinition.h"
#include "actionsetlayerdefinition.h"

namespace QSteamworks {

class ActionSetDefinition {
  Q_GADGET

public:
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
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::ActionSetDefinition *)
