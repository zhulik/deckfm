#pragma once

#include <QJsonObject>

#include "actionsetdefinition.h"

namespace QSteamworks {
namespace QSteamInput {

class IGA {
  Q_GADGET
  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionSetDefinition> actionSets READ actionSets CONSTANT)

public:
  IGA(){};
  IGA(const QJsonObject &definition);

  QList<ActionSetDefinition> actionSets() const;

private:
  QList<ActionSetDefinition> m_actionSets;
};
} // namespace QSteamInput
}; // namespace QSteamworks

Q_DECLARE_METATYPE(QSteamworks::QSteamInput::IGA)
