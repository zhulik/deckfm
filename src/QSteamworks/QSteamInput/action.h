#pragma once

#include <QObject>

#include "actiondefinition.h"
#include "steam/isteaminput.h"

namespace QSteamworks {
namespace QSteamInput {

class Action {
  Q_GADGET
  Q_PROPERTY(InputHandle_t handle READ handle CONSTANT)
  Q_PROPERTY(const QSteamworks::QSteamInput::ActionDefinition actionDefinition READ actionDefinition CONSTANT)
  Q_PROPERTY(QString localizedName READ localizedName CONSTANT)
  Q_PROPERTY(QStringList glyphs READ glyphs CONSTANT)
  Q_PROPERTY(QStringList origins READ origins CONSTANT)

public:
  Action(){};
  Action(InputHandle_t handle, ActionDefinition definition, const QString &localizedName, const QStringList &origins,
         const QStringList &glyphs);

  const QSteamworks::QSteamInput::ActionDefinition actionDefinition() const;
  InputHandle_t handle() const;
  const QString &localizedName() const;
  const QStringList &glyphs() const;
  const QStringList &origins() const;

  bool operator==(const Action &other);

private:
  InputHandle_t m_handle = 0;

  ActionDefinition m_definition;
  QString m_localizedName;
  QStringList m_origins;
  QStringList m_glyphs;
};
}; // namespace QSteamInput
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::QSteamInput::Action)
