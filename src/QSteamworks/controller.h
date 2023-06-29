#pragma once

#include <QObject>

#include "actionset.h"
#include "iga.h"
#include "steam/steam_api.h"

namespace QSteamworks {
class Controller : public QObject {
  Q_OBJECT
  Q_PROPERTY(InputHandle_t handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString image READ image CONSTANT)
  Q_PROPERTY(QList<QSteamworks::ActionSet> actionSets READ actionSets CONSTANT)

public:
  Controller(InputHandle_t, const QString &, const IGA &, QObject *parent = nullptr);

  InputHandle_t handle() const;

  const QString &name() const;

  const QString &image() const;

  QList<QSteamworks::ActionSet> actionSets() const;

private:
  InputHandle_t m_handle = 0;
  QString m_name;
  QString m_image;
  QList<ActionSet> m_actionSets;

  QList<Action> getActions(InputActionSetHandle_t actionSetHandle, const QList<ActionDefinition> &actions) const;
  QList<ActionSetLayer> getActionSetLayers(const QList<ActionSetLayerDefinition> &definitions) const;
};

} // namespace QSteamworks
