#pragma once

#include <QObject>

#include "action.h"
#include "actionset.h"
#include "actionsetlayer.h"
#include "iga.h"
#include "inputevent.h"
#include "qobjectdefs.h"
#include "steam/isteamcontroller.h"
#include "steam/steam_api.h"

#include "QSteamworks/steaminput.h"

namespace QSteamworks {
namespace QSteamInput {

class Controller : public QObject {
  Q_OBJECT
  Q_PROPERTY(InputHandle_t handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString image READ image CONSTANT)
  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionSet> actionSets READ actionSets NOTIFY actionSetsChanged)
  Q_PROPERTY(QSteamworks::QSteamInput::ActionSet actionSet READ actionSet WRITE setActionSet NOTIFY actionSetChanged)
  Q_PROPERTY(QVariantMap actionStates READ actionStates NOTIFY actionStatesChanged)

  Q_PROPERTY(QList<QSteamworks::QSteamInput::ActionSetLayer> activeActionSetLayers READ activeActionSetLayers WRITE
                 setActiveActionSetLayers NOTIFY activeActionSetLayersChanged)

public:
  Controller(InputHandle_t, const QString &, const IGA &, QObject *parent = nullptr);

  InputHandle_t handle() const;
  const QString &name() const;
  const QString &image() const;
  QList<QSteamworks::QSteamInput::ActionSet> actionSets() const;

  QSteamworks::QSteamInput::ActionSet actionSet() const;
  void setActionSet(const QSteamworks::QSteamInput::ActionSet &newActionSet);

  QVariantMap actionStates() const;

  QList<QSteamworks::QSteamInput::ActionSetLayer> activeActionSetLayers() const;

  Q_INVOKABLE
  void loadActions();

  Q_INVOKABLE
  void showBindingPanel() const;

  Q_INVOKABLE
  void activateActionSetLayer(const QSteamworks::QSteamInput::ActionSetLayer &layer);

  Q_INVOKABLE
  void deactivateActionSetLayer(const QSteamworks::QSteamInput::ActionSetLayer &layer);

  Q_INVOKABLE
  void deactivateAllActionSetLayers();

  Q_INVOKABLE
  QSteamworks::QSteamInput::ActionSet actionSetByName(const QString &);

  Q_INVOKABLE
  void stopAnalogActionMomentum(const QSteamworks::QSteamInput::Action &);

  // TODO: only left trackpad is currently supported
  Q_INVOKABLE
  void triggerRepeatedHapticPulse(unsigned short usDurationMicroSec, unsigned short usOffMicroSec,
                                  unsigned short unRepeat);

  void setActiveActionSetLayers(const QList<QSteamworks::QSteamInput::ActionSetLayer> &newActiveActionSetLayers);

signals:
  void actionSetsChanged();
  void actionSetChanged();

  void inputEvent(const QSteamworks::QSteamInput::InputEvent &event);

  void actionStatesChanged();
  void activeActionSetLayersChanged();

private:
  InputHandle_t m_handle = 0;
  QString m_name;
  QString m_image;
  QMap<ControllerActionSetHandle_t, ActionSet> m_actionSets;
  IGA m_iga;
  QVariantMap m_actionStates;
  QSteamworks::QSteamInput::ActionSet m_actionSet;

  QList<Action> getActions(InputActionSetHandle_t actionSetHandle, const QList<ActionDefinition> &actions) const;
  QList<ActionSetLayer> getActionSetLayers(const QList<ActionSetLayerDefinition> &definitions) const;
  void updateActionStates(const Action &action, bool digitalState, float analogX, float analogY);

  friend class QSteamworks::SteamInput;
  void onActionEvent(SteamInputActionEvent_t *event);
};
} // namespace QSteamInput
} // namespace QSteamworks
