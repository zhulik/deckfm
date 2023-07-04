#include "actionsetlayer.h"
#include "collections.h"

using namespace QSteamworks;
using namespace QSteamworks::QSteamInput;

ActionSetLayer::ActionSetLayer() {}

ActionSetLayer::ActionSetLayer(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions)
    : m_handle(handle), m_name(name), m_actions(actions) {}

InputHandle_t ActionSetLayer::handle() const { return m_handle; }

const QString &ActionSetLayer::name() const { return m_name; }

const QList<Action> ActionSetLayer::actions() const { return m_actions; }

QVariantMap ActionSetLayer::qmlActions() const {
  // TODO: map
  QVariantMap result;
  foreach (auto &action, m_actions) {
    result[action.name()] = QVariant::fromValue(action);
  }
  return result;
}

Action ActionSetLayer::actionByHandle(InputHandle_t handle, bool digital) const {
  return findBy(m_actions,
                [handle, digital](auto action) { return action.handle() == handle && action.digital() == digital; });
}

Action ActionSetLayer::actionByName(const QString &name, bool digital) const {
  return findBy(m_actions,
                [name, digital](auto action) { return action.name() == name && action.digital() == digital; });
}
