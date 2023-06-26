#pragma once

#include <QObject>
#include <QVariant>

#include "steam/steam_api.h"

#include "action.h"

namespace QSteamworks {

class ActionSetLayer {
  Q_GADGET
  Q_PROPERTY(InputHandle_t handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QVariantMap actions READ qmlActions CONSTANT)

public:
  ActionSetLayer();
  ActionSetLayer(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions);

  InputHandle_t handle() const;
  const QString &name() const;

  const QList<Action> actions() const;

  QVariantMap qmlActions() const;

  bool operator==(const ActionSetLayer &other) const;

private:
  InputActionSetHandle_t m_handle;
  QString m_name;

protected:
  QList<Action> m_actions;
};
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::ActionSetLayer)
