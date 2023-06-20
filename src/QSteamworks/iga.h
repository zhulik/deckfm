#pragma once

#include <QJsonObject>

#include "actiondefinition.h"
#include "actionsetdefinition.h"

namespace QSteamworks {

class IGA {
  Q_GADGET
  Q_PROPERTY(QStringList actionSets READ qmlActionSets CONSTANT)
  Q_PROPERTY(QStringList actions READ qmlActions CONSTANT)
  Q_PROPERTY(QStringList actionSetLayers READ qmlActionSetLayers CONSTANT)

public:
  IGA(){};
  IGA(const QJsonObject &definition);

  const QMap<QString, ActionSetDefinition> &actionSets() const;

  QStringList qmlActionSets() const;

  Q_INVOKABLE
  QStringList actionsForSet(const QString &) const;

  Q_INVOKABLE
  QVariantList qmlActionsForSet(const QString &) const;

  QStringList qmlActions() const;

  Q_INVOKABLE
  ActionDefinition actionDefinition(const QString &) const;

  QStringList qmlActionSetLayers() const;

private:
  QMap<QString, ActionSetDefinition> m_actionSets;
  QMap<QString, ActionSetLayerDefinition> m_actionSetLayers;
  QList<ActionDefinition> m_actions;
};
}; // namespace QSteamworks

Q_DECLARE_METATYPE(QSteamworks::IGA)
