#pragma once

#include <QObject>
#include <QVariant>

#include "steam/steam_api.h"

#include "actionsetlayer.h"

namespace QSteamworks {

class ActionSet : public ActionSetLayer {
  Q_GADGET
  Q_PROPERTY(QList<QSteamworks::ActionSetLayer> layers READ layers CONSTANT)

public:
  ActionSet() : ActionSetLayer(){};
  ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions,
            const QList<ActionSetLayer> &layers);

  const QList<ActionSetLayer> &layers() const;

  bool operator==(const ActionSet &other) const { return handle() == other.handle(); }

private:
  QList<ActionSetLayer> m_layers;
};
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::ActionSet)
