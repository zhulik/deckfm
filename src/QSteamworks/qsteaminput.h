#pragma once

#include "actiondefinition.h"
#include "controller.h"

#include <QObject>

#include "iga.h"

#include "steam/steam_api.h"

namespace QSteamworks {

class QSteamAPI;

struct Action {
  unsigned long long handle;

  ActionDefinition definition;
  QString localizedName;
  QStringList origins;
  QStringList glyphs;

  bool operator==(const Action &other) { return handle == other.handle; }
};

struct ActionSet {
  InputActionSetHandle_t handle;
  QString name;
  QList<Action> actions;
};

class QSteamInput : public QObject {
  Q_OBJECT
  Q_PROPERTY(QSteamworks::IGA iga READ iga CONSTANT)
  Q_PROPERTY(QVariantList controllers READ qmlControllers NOTIFY qmlControllersChanged)
  Q_PROPERTY(QSteamworks::Controller currentController READ currentController WRITE setCurrentController NOTIFY
                 currentControllerChanged)

  STEAM_CALLBACK(QSteamInput, onControllerConnected, SteamInputDeviceConnected_t);
  STEAM_CALLBACK(QSteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t);

public:
  explicit QSteamInput(const QString &vdf, QSteamAPI *parent = nullptr);
  virtual ~QSteamInput();

  Q_INVOKABLE
  void runFrame();

  const IGA &iga() const;

  QVariantList qmlControllers() const;

  const Controller &currentController() const;

signals:
  void qmlControllersChanged();
  void digitalEvent();
  void analogEvent();

  void currentControllerChanged();

private:
  QString m_vdf;
  IGA m_iga;
  QSet<Controller> m_controllers;
  Controller m_currentController;
  QList<ActionSet> m_actionSets;

  static QSteamInput *instance();
  static QSteamInput *m_instance;
  void onActionEvent(SteamInputActionEvent_t *event);

  void setCurrentController(const Controller &newCurrentController);
  void updateActionSets();
  QList<Action> getActions(InputActionSetHandle_t actionSetHandle, const QList<ActionDefinition> &actions) const;
};
} // namespace QSteamworks
