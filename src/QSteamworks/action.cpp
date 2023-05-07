#include "action.h"

using namespace QSteamworks;

Action::Action(unsigned long long handle, ActionDefinition *definition, const QString &localizedName,
               const QStringList &origins, const QStringList &glyphs)
    : m_handle(handle), m_definition(definition), m_localizedName(localizedName), m_origins(origins), m_glyphs(glyphs) {
}

const ActionDefinition *Action::actionDefinition() const { return m_definition; }

unsigned long long Action::handle() const { return m_handle; }

const QString &Action::localizedName() const { return m_localizedName; }

const QStringList &Action::glyphs() const { return m_glyphs; }

const QStringList &Action::origins() const { return m_origins; }

bool Action::operator==(const Action &other) { return m_handle == other.m_handle; }
