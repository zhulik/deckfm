#include "actionset.h"
#include "actionsetlayer.h"

#include "collections.h"

using namespace QSteamworks::QSteamInput;

ActionSet::ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions,
                     const QList<ActionSetLayer> &layers)
    : ActionSetLayer(handle, name, actions), m_layers(layers) {
  m_actions = actions;
  foreach (auto &layer, m_layers) {
    m_actions.append(layer.actions());
  }
}

const QList<ActionSetLayer> &ActionSet::layers() const { return m_layers; }

Action ActionSet::actionByHandle(InputHandle_t handle, bool digital) const {
  auto a = ActionSetLayer::actionByHandle(handle, digital);
  if (a.handle()) {
    return a;
  }
  foreach (auto &layer, m_layers) {
    auto a = layer.actionByHandle(handle, digital);
    if (a.handle()) {
      return a;
    }
  }
  return Action();
}

Action ActionSet::actionByName(const QString &name, bool digital) const {
  auto a = ActionSetLayer::actionByName(name, digital);
  if (a.handle()) {
    return a;
  }
  foreach (auto &layer, m_layers) {
    auto a = layer.actionByName(name, digital);
    if (a.handle()) {
      return a;
    }
  }
  return Action();
}

ActionSetLayer ActionSet::layerByName(const QString &name) const {
  return findBy(m_layers, [name](auto l) { return l.name() == name; });
}
