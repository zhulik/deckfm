#pragma once

#include "actionsetlayerdefinition.h"
#include <QJsonObject>
#include <actiondefinition.h>
#include <actionsetdefinition.h>

namespace QSteamworks {

class IGA {
  Q_GADGET
  Q_PROPERTY(QStringList actionSets READ qmlActionSets CONSTANT)
  Q_PROPERTY(QStringList actions READ qmlActions CONSTANT)
  Q_PROPERTY(QStringList actionSetLayers READ qmlActionSetLayers CONSTANT)

public:
  IGA(const QJsonObject &definition);
  virtual ~IGA();

  const QMap<QString, ActionSetDefinition *> &actionSets() const;

  QStringList qmlActionSets() const;

  Q_INVOKABLE
  QStringList actionsForSet(const QString &) const;

  Q_INVOKABLE
  QVariantList qmlActionsForSet(const QString &) const;

  QStringList qmlActions() const;

  Q_INVOKABLE
  QSteamworks::ActionDefinition actionDefinition(const QString &) const;

  QStringList qmlActionSetLayers() const;

private:
  QMap<QString, ActionSetDefinition *> m_actionSets;
  QMap<QString, ActionSetLayerDefinition *> m_actionSetLayers;
};
}; // namespace QSteamworks

Q_DECLARE_METATYPE(QSteamworks::IGA *)
