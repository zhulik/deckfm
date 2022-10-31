#include "actionsetdefinition.h"

using namespace QSteamworks;

ActionSetDefinition::ActionSetDefinition() {}

ActionSetDefinition::ActionSetDefinition(const QString &name, const QList<ActionDefinition> &actions)
    : m_name(name), m_actions(actions) {}

const QList<ActionDefinition> &ActionSetDefinition::actions() const { return m_actions; }

const QString &ActionSetDefinition::name() const { return m_name; }
