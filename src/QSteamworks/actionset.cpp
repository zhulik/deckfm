#include "actionset.h"
#include "actionsetlayer.h"

using namespace QSteamworks;

ActionSet::ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions,
                     const QList<ActionSetLayer> &layers)
    : ActionSetLayer(handle, name, actions), m_layers(layers) {
  foreach (auto &layer, m_layers) {
    m_actions.append(layer.actions());
  }
}

const QList<ActionSetLayer> &ActionSet::layers() const { return m_layers; }

QStringList ActionSet::qmlLayers() const {
  QStringList result;
  foreach (auto &layer, m_layers) {
    result << layer.name();
  }
  return result;
}
