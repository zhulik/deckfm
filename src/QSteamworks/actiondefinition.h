#pragma once

#include <QObject>

namespace QSteamworks {
class ActionDefinition {
  Q_GADGET
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString type READ type CONSTANT)
  Q_PROPERTY(QString actionSet READ actionSet CONSTANT)
  Q_PROPERTY(bool isDigital READ isDigital CONSTANT)

public:
  ActionDefinition(){};
  ActionDefinition(const QString &name, const QString &type, const QString &actionSet, bool);

  bool isDigital() const;

  const QString &name() const;

  const QString &type() const;

  const QString &actionSet() const;

private:
  bool m_isDigital;
  QString m_name;
  QString m_type;
  QString m_actionSet;
};
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::ActionDefinition)
