#include "actionsetlayer.h"

using namespace QSteamworks;

ActionSetLayer::ActionSetLayer() {}

ActionSetLayer::ActionSetLayer(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions)
    : m_handle(handle), m_name(name), m_actions(actions) {}

unsigned long long ActionSetLayer::handle() const { return m_handle; }

const QString &ActionSetLayer::name() const { return m_name; }

const QList<Action> ActionSetLayer::actions() const { return m_actions; }

QVariantMap ActionSetLayer::qmlActions() const {
  QVariantMap result;
  foreach (auto &action, m_actions) {
    result[action.actionDefinition().name()] = QVariant::fromValue(action);
  }
  return result;
}

bool ActionSetLayer::operator==(const ActionSetLayer &other) const { return m_handle == other.m_handle; }
