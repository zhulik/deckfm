#pragma once

#include "actiondefinition.h"
#include "controller.h"

#include <QObject>

#include "iga.h"

#include "qobjectdefs.h"
#include "steam/steam_api.h"

namespace QSteamworks {

class QSteamAPI;

class Action {
  Q_GADGET
  Q_PROPERTY(unsigned long long handle READ handle CONSTANT)
  Q_PROPERTY(QSteamworks::ActionDefinition actionDefinition READ actionDefinition CONSTANT)
  Q_PROPERTY(QString localizedName READ localizedName CONSTANT)
  Q_PROPERTY(QStringList glyphs READ glyphs CONSTANT)
  Q_PROPERTY(QStringList origins READ origins CONSTANT)

public:
  Action(){};
  Action(unsigned long long handle, const ActionDefinition &definition, const QString &localizedName,
         const QStringList &origins, const QStringList &glyphs)
      : m_handle(handle), m_definition(definition), m_localizedName(localizedName), m_origins(origins),
        m_glyphs(glyphs) {}

  const QSteamworks::ActionDefinition &actionDefinition() const { return m_definition; }
  unsigned long long handle() const { return m_handle; }
  const QString &localizedName() const { return m_localizedName; }
  const QStringList &glyphs() const { return m_glyphs; }
  const QStringList &origins() const { return m_origins; }

  bool operator==(const Action &other) { return m_handle == other.m_handle; }

private:
  unsigned long long m_handle;

  ActionDefinition m_definition;
  QString m_localizedName;
  QStringList m_origins;
  QStringList m_glyphs;
};

class ActionSet {
  Q_GADGET
  Q_PROPERTY(unsigned long long handle READ handle CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QVariantList actions READ qmlActions CONSTANT)

public:
  ActionSet(){};
  ActionSet(InputActionSetHandle_t handle, const QString &name, const QList<Action> &actions)
      : m_handle(handle), m_name(name), m_actions(actions){};

  unsigned long long handle() const { return m_handle; }
  const QString &name() const { return m_name; }

  const QList<Action> actions() const { return m_actions; }

  QVariantList qmlActions() const {
    QVariantList result;
    foreach (auto &action, m_actions) {
      result << QVariant::fromValue(action);
    }
    return result;
  }

  bool operator==(const ActionSet &other) const { return m_handle == other.m_handle; }

private:
  InputActionSetHandle_t m_handle;
  QString m_name;
  QList<Action> m_actions;
};

class InputEvent {
  Q_GADGET
  Q_PROPERTY(QString type READ type CONSTANT)

  Q_PROPERTY(QSteamworks::Controller controller READ controller CONSTANT)
  Q_PROPERTY(QSteamworks::Action action READ action CONSTANT)

  Q_PROPERTY(bool digitalState READ digitalState CONSTANT)
  Q_PROPERTY(float analogX READ analogX CONSTANT)
  Q_PROPERTY(float analogY READ analogY CONSTANT)

public:
  InputEvent() {}

  InputEvent(const QString &type, const Controller &controller, const Action &action, bool digitalState, float analogX,
             float analogY)
      : m_type(type), m_controller(controller), m_action(action), m_digitalState(digitalState), m_analogX(analogX),
        m_analogY(analogY) {}

  const QString &type() const { return m_type; }
  Controller controller() const { return m_controller; }

  Action action() const { return m_action; }

  bool digitalState() const { return m_digitalState; }
  float analogX() const { return m_analogX; }
  float analogY() const { return m_analogY; }

private:
  QString m_type;
  Controller m_controller;
  Action m_action;
  bool m_digitalState = false;
  float m_analogX = 0;
  float m_analogY = 0;
};

class QSteamInput : public QObject {
  Q_OBJECT
  Q_PROPERTY(QSteamworks::IGA iga READ iga CONSTANT)
  Q_PROPERTY(QVariantList controllers READ qmlControllers NOTIFY qmlControllersChanged)
  Q_PROPERTY(QSteamworks::Controller currentController READ currentController WRITE setCurrentController NOTIFY
                 currentControllerChanged)

  Q_PROPERTY(QVariantList actionSets READ qmlActionSets NOTIFY actionSetsChanged)

  Q_PROPERTY(QString actionSet READ qmlActionSet WRITE qmlSetActionSet NOTIFY actionSetChanged)

  STEAM_CALLBACK(QSteamInput, onControllerConnected, SteamInputDeviceConnected_t);
  STEAM_CALLBACK(QSteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t);
  STEAM_CALLBACK(QSteamInput, onConfigurationLoaded, SteamInputConfigurationLoaded_t);

public:
  explicit QSteamInput(const QString &vdf, QSteamAPI *parent = nullptr);
  virtual ~QSteamInput();

  Q_INVOKABLE
  void runFrame();

  Q_INVOKABLE
  virtual bool showBindingPanel(unsigned long long inputHandle) const;

  const IGA &iga() const;

  QVariantList qmlControllers() const;

  const Controller &currentController() const;
  QVariantList qmlActionSets() const;

  const QSteamworks::ActionSet &actionSet() const;

  const QString &qmlActionSet() const;
  void qmlSetActionSet(const QString &newActionSet);

signals:
  void qmlControllersChanged();
  void inputEvent(QSteamworks::InputEvent);

  void currentControllerChanged();
  void actionSetsChanged();
  void configurationLoaded();

  void actionSetChanged();

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

  Action action(const QString &, bool = true) const;
  Action action(unsigned long long, bool = true) const;

  QSteamworks::ActionSet m_actionSet;
  void setActionSet(const QSteamworks::ActionSet &newActionSet);
};
} // namespace QSteamworks

Q_DECLARE_METATYPE(QSteamworks::ActionSet)
Q_DECLARE_METATYPE(QSteamworks::Action)
Q_DECLARE_METATYPE(QSteamworks::InputEvent)
