#include "actionsetlayerdefinition.h"

using namespace QSteamworks::QSteamInput;

ActionSetLayerDefinition::ActionSetLayerDefinition(const QString &name, const QList<ActionDefinition> &actions)
    : m_name(name), m_actions(actions) {}

const QString &ActionSetLayerDefinition::name() const { return m_name; }

const QList<ActionDefinition> &ActionSetLayerDefinition::actions() const { return m_actions; }
