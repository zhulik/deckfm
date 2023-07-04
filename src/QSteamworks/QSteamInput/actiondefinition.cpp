#include "actiondefinition.h"

using namespace QSteamworks::QSteamInput;

ActionDefinition::ActionDefinition(const QString &name, const QString &type, const QString &actionSet, bool isDigital)
    : m_name(name), m_type(type), m_actionSet(actionSet), m_isDigital(isDigital) {}

bool ActionDefinition::isDigital() const { return m_isDigital; }

const QString &ActionDefinition::name() const { return m_name; }

const QString &ActionDefinition::type() const { return m_type; }

const QString &ActionDefinition::actionSet() const { return m_actionSet; }
