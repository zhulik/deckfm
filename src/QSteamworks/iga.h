#pragma once

#include <QJsonObject>
#include <actiondefinition.h>

namespace QSteamworks {

class IGA {
  Q_GADGET
  Q_PROPERTY(QStringList actionSets READ qmlActionSets CONSTANT)
  Q_PROPERTY(QStringList actions READ qmlActions CONSTANT)

public:
  IGA();
  IGA(const QJsonObject &definition);

  const QMap<QString, QList<ActionDefinition>> &actionSets() const;

  QStringList qmlActionSets() const;

  Q_INVOKABLE
  QStringList actionsForSet(const QString &) const;

  Q_INVOKABLE
  QVariantList qmlActionsForSet(const QString &) const;

  QStringList qmlActions() const;

  Q_INVOKABLE
  QSteamworks::ActionDefinition actionDefinition(const QString &) const;

private:
  QMap<QString, QList<ActionDefinition>> m_actionSets;
};
}; // namespace QSteamworks

Q_DECLARE_METATYPE(QSteamworks::IGA)
