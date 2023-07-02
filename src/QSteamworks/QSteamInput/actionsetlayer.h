#pragma once

#include <QObject>
#include <QVariant>

#include "qobjectdefs.h"
#include "steam/steam_api.h"

#include "action.h"

namespace QSteamworks {
namespace QSteamInput {

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

  Q_INVOKABLE
  virtual QSteamworks::QSteamInput::Action actionByHandle(InputHandle_t handle, bool digital) const;

  Q_INVOKABLE
  virtual QSteamworks::QSteamInput::Action actionByName(const QString &name, bool digital) const;

protected:
  InputActionSetHandle_t m_handle = 0;
  QString m_name;

  QList<Action> m_actions;
};
} // namespace QSteamInput
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::QSteamInput::ActionSetLayer)
