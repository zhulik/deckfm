#include "actionset.h"

QSteamworks::ActionSet::ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions)
    : m_handle(handle), m_name(name), m_actions(actions) {}

unsigned long long QSteamworks::ActionSet::handle() const { return m_handle; }

const QString &QSteamworks::ActionSet::name() const { return m_name; }

const QList<QSteamworks::Action> QSteamworks::ActionSet::actions() const { return m_actions; }

QVariantList QSteamworks::ActionSet::qmlActions() const {

  QVariantList result;
  foreach (auto &action, m_actions) {
    result << QVariant::fromValue(action);
  }
  return result;
}

bool QSteamworks::ActionSet::operator==(const ActionSet &other) const { return m_handle == other.m_handle; }
