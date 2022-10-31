#pragma once

#include <QObject>
#include <QVariant>

#include "steam/steam_api.h"

#include "action.h"

namespace QSteamworks {

class ActionSet {
  Q_GADGET
  Q_PROPERTY(unsigned long long handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QVariantList actions READ qmlActions CONSTANT)

public:
  ActionSet(){};
  ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions);

  unsigned long long handle() const;
  const QString &name() const;

  const QList<Action> actions() const;

  QVariantList qmlActions() const;

  bool operator==(const ActionSet &other) const;

private:
  InputActionSetHandle_t m_handle;
  QString m_name;
  QList<Action> m_actions;
};
} // namespace QSteamworks
