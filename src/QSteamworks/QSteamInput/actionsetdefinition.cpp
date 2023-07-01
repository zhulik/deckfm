#include "actionsetdefinition.h"

using namespace QSteamworks;
using namespace QSteamworks::QSteamInput;

ActionSetDefinition::ActionSetDefinition(const QString &name, const QList<ActionDefinition> &actions,
                                         const QList<ActionSetLayerDefinition> &layers)
    : m_name(name), m_actions(actions), m_layers(layers) {}

const QList<ActionDefinition> &ActionSetDefinition::actions() const { return m_actions; }

const QString &ActionSetDefinition::name() const { return m_name; }

const QList<ActionSetLayerDefinition> &ActionSetDefinition::layers() const { return m_layers; }
