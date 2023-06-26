#pragma once

#include "controller.h"

#include <QObject>

#include "iga.h"

#include "qobjectdefs.h"
#include "steam/steam_api.h"

#include "actionset.h"
#include "inputevent.h"

namespace QSteamworks {

class QSteamAPI;

class QSteamInput : public QObject {
  Q_OBJECT

  Q_PROPERTY(QSteamworks::IGA iga READ iga NOTIFY igaChanged)
  Q_PROPERTY(QSteamworks::Controller currentController READ currentController WRITE setCurrentController NOTIFY
                 currentControllerChanged)
  Q_PROPERTY(QVariantList actionSets READ qmlActionSets NOTIFY actionSetsChanged)
  Q_PROPERTY(QString actionSet READ qmlActionSet WRITE setActionSet NOTIFY actionSetChanged)

  Q_PROPERTY(QVariantMap actionStates READ actionStates NOTIFY actionStatesChanged)
  Q_PROPERTY(QString igaPath READ igaPath WRITE setIgaPath NOTIFY igaPathChanged)

  Q_PROPERTY(QString defaultActionSet READ defaultActionSet WRITE setDefaultActionSet NOTIFY defaultActionSetChanged)
  Q_PROPERTY(QString defaultActionSetLayer READ defaultActionSetLayer WRITE setDefaultActionSetLayer NOTIFY
                 defaultActionSetLayerChanged)

  Q_PROPERTY(QSteamworks::ActionSet currentActionSet READ currentActionSet NOTIFY actionSetChanged)

  Q_PROPERTY(QSteamworks::ActionSetLayer currentActionSetLayer READ currentActionSetLayer NOTIFY actionSetLayerChanged)
  Q_PROPERTY(QString actionSetLayer READ qmlActionSetLayer WRITE setActionSetLayer NOTIFY actionSetLayerChanged)

  STEAM_CALLBACK(QSteamInput, onControllerConnected, SteamInputDeviceConnected_t);
  STEAM_CALLBACK(QSteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t);
  STEAM_CALLBACK(QSteamInput, onConfigurationLoaded, SteamInputConfigurationLoaded_t);

public:
  explicit QSteamInput(QObject *parent = nullptr);
  virtual ~QSteamInput();

  Q_INVOKABLE
  void runFrame();

  Q_INVOKABLE
  virtual bool showBindingPanel(unsigned long long inputHandle) const;

  Q_INVOKABLE
  virtual bool showBindingPanel() const;

  Q_INVOKABLE
  virtual void stopAnalogActionMomentum(const QString &actionName) const;

  Q_INVOKABLE
  void triggerSimpleHapticEvent(const QString &location, unsigned char nIntensity, char nGainDB,
                                unsigned char nOtherIntensity, char nOtherGainDB) const;

  IGA iga() const;

  const Controller &currentController() const;
  QVariantList qmlActionSets() const;

  const QSteamworks::ActionSet &actionSet() const;

  const QString &qmlActionSet() const;
  void setActionSet(const QString &newActionSet);

  const QVariantMap &actionStates() const;

  const QString &igaPath() const;
  void setIgaPath(const QString &newIgaPath);

  const QString &defaultActionSet() const;
  void setDefaultActionSet(const QString &newDefaultActionSet);

  const QSteamworks::ActionSet &currentActionSet() const;

  const QSteamworks::ActionSetLayer &currentActionSetLayer() const;

  const QString &qmlActionSetLayer() const;
  void setActionSetLayer(const QString &newActionSetLayer);

  const QString &defaultActionSetLayer() const;
  void setDefaultActionSetLayer(const QString &newDefaultActionSetLayer);

signals:
  void inputEvent(QSteamworks::InputEvent);
  void pressedEvent(QSteamworks::InputEvent);
  void releasedEvent(QSteamworks::InputEvent);
  void analogEvent(QSteamworks::InputEvent);

  void currentControllerChanged();
  void actionSetsChanged();
  void configurationLoaded();

  void actionSetChanged();

  void actionStatesChanged();

  void igaChanged();

  void igaPathChanged();

  void defaultActionSetChanged();

  void actionSetLayerChanged();

  void defaultActionSetLayerChanged();

private:
  IGA m_iga;
  QSet<Controller> m_controllers;
  Controller m_currentController;
  QList<ActionSet> m_actionSets;

  static QSteamInput *instance();
  static QSteamInput *m_instance;
  void onActionEvent(SteamInputActionEvent_t *event);

  const QSteamworks::Action &actionByHandle(unsigned long long, bool = true) const;
  const QSteamworks::Action actionByName(const QString &) const;

  void setCurrentController(const Controller &newCurrentController);
  void updateActionSets();
  QList<Action> getActions(InputActionSetHandle_t actionSetHandle, const QList<ActionDefinition> &actions) const;

  ActionSet m_actionSet;
  void setActionSet(const QSteamworks::ActionSet &newActionSet);

  QVariantMap m_actionStates;
  void updateActionStates(const Action &action, bool digitalState, float analogX, float analogY);
  void sendInputEvents(InputEvent iEvent);
  QString m_igaPath;
  QString m_defaultActionSet;

  QList<ActionSetLayer> getActionSetLayers(const QList<ActionSetLayerDefinition> &) const;
  QSteamworks::ActionSetLayer m_currentActionSetLayer;
  QString m_defaultActionSetLayer;
};
} // namespace QSteamworks
