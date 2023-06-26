#pragma once

#include <QObject>

#include "actiondefinition.h"
#include "steam/isteaminput.h"

namespace QSteamworks {

class Action {
  Q_GADGET
  Q_PROPERTY(InputHandle_t handle READ handle CONSTANT)
  Q_PROPERTY(const QSteamworks::ActionDefinition actionDefinition READ actionDefinition CONSTANT)
  Q_PROPERTY(QString localizedName READ localizedName CONSTANT)
  Q_PROPERTY(QStringList glyphs READ glyphs CONSTANT)
  Q_PROPERTY(QStringList origins READ origins CONSTANT)

public:
  Action(){};
  Action(InputHandle_t handle, ActionDefinition definition, const QString &localizedName, const QStringList &origins,
         const QStringList &glyphs);

  const QSteamworks::ActionDefinition actionDefinition() const;
  InputHandle_t handle() const;
  const QString &localizedName() const;
  const QStringList &glyphs() const;
  const QStringList &origins() const;

  bool operator==(const Action &other);

private:
  InputHandle_t m_handle;

  ActionDefinition m_definition;
  QString m_localizedName;
  QStringList m_origins;
  QStringList m_glyphs;
};
} // namespace QSteamworks
Q_DECLARE_METATYPE(QSteamworks::Action)
