#pragma once

#include <QObject>

#include "iga.h"

#include "qglobal.h"
#include "qobjectdefs.h"
#include "steam/steam_api.h"

#include "inputevent.h"

namespace QSteamworks {

class QSteamAPI;
class Controller;

class ControllerMap : public QMap<InputHandle_t, QSteamworks::Controller *> {};

class QSteamInput : public QObject {
  Q_OBJECT

  Q_PROPERTY(QSteamworks::IGA iga READ iga NOTIFY igaChanged)

  Q_PROPERTY(QList<QSteamworks::Controller *> controllers READ controllers NOTIFY controllersChanged)

  Q_PROPERTY(QVariantMap actionStates READ actionStates NOTIFY actionStatesChanged)
  Q_PROPERTY(QString igaPath READ igaPath WRITE setIgaPath NOTIFY igaPathChanged)

  STEAM_CALLBACK(QSteamInput, onControllerConnected, SteamInputDeviceConnected_t);
  STEAM_CALLBACK(QSteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t);
  STEAM_CALLBACK(QSteamInput, onConfigurationLoaded, SteamInputConfigurationLoaded_t);

public:
  explicit QSteamInput(QObject *parent = nullptr);
  virtual ~QSteamInput();

  Q_INVOKABLE
  void runFrame();

  IGA iga() const;

  const QVariantMap &actionStates() const;

  const QString &igaPath() const;
  void setIgaPath(const QString &newIgaPath);

  QList<QSteamworks::Controller *> controllers() const;

signals:
  void inputEvent(QSteamworks::InputEvent);
  void pressedEvent(QSteamworks::InputEvent);
  void releasedEvent(QSteamworks::InputEvent);
  void analogEvent(QSteamworks::InputEvent);

  void currentControllerChanged();
  void configurationLoaded();

  void actionStatesChanged();

  void igaChanged();
  void igaPathChanged();

  void controllersChanged();

private:
  IGA m_iga;
  ControllerMap m_controllers;

  static QSteamInput *instance();
  static QSteamInput *m_instance;

  void onActionEvent(SteamInputActionEvent_t *event);

  const QSteamworks::Action &actionByHandle(InputHandle_t, bool = true) const;
  const QSteamworks::Action actionByName(const QString &) const;

  QVariantMap m_actionStates;
  void updateActionStates(const Action &action, bool digitalState, float analogX, float analogY);
  void sendInputEvents(InputEvent iEvent);
  QString m_igaPath;
};
} // namespace QSteamworks

Q_DECLARE_METATYPE(InputHandle_t)
