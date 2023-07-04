#pragma once

#include <QObject>
#include <QVariant>

#include "actionsetlayer.h"

namespace QSteamworks {

namespace QSteamInput {

class ActionSet : public ActionSetLayer {
  Q_GADGET
  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionSetLayer> layers READ layers CONSTANT)

public:
  ActionSet() : ActionSetLayer() {}
  ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions,
            const QList<ActionSetLayer> &layers);

  const QList<ActionSetLayer> &layers() const;

  bool operator==(const ActionSet &other) const { return handle() == other.handle(); }

  Q_INVOKABLE
  virtual Action actionByHandle(InputHandle_t handle, bool digital) const override;

  Q_INVOKABLE
  virtual QSteamworks::QSteamInput::Action actionByName(const QString &name, bool digital) const override;

  Q_INVOKABLE
  virtual QSteamworks::QSteamInput::ActionSetLayer layerByName(const QString &name) const;

private:
  QList<ActionSetLayer> m_layers;
};
} // namespace QSteamInput
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::QSteamInput::ActionSet)
