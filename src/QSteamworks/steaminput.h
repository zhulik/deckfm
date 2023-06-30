#pragma once

#include <QObject>

#include "QSteamInput/iga.h"

#include "qglobal.h"
#include "qobjectdefs.h"
#include "steam/steam_api.h"

#include "QSteamInput/inputevent.h"

namespace QSteamworks {

class SteamAPI;
class Controller;

class SteamInput : public QObject {
  Q_OBJECT

  Q_PROPERTY(QSteamworks::IGA iga READ iga NOTIFY igaChanged)
  Q_PROPERTY(QList<QSteamworks::Controller *> controllers READ controllers NOTIFY controllersChanged)
  Q_PROPERTY(QString igaPath READ igaPath WRITE setIgaPath NOTIFY igaPathChanged)

  Q_PROPERTY(QSteamworks::Controller *lastController READ lastController NOTIFY lastControllerChanged)

  STEAM_CALLBACK(SteamInput, onControllerConnected, SteamInputDeviceConnected_t);
  STEAM_CALLBACK(SteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t);
  STEAM_CALLBACK(SteamInput, onConfigurationLoaded, SteamInputConfigurationLoaded_t);

public:
  explicit SteamInput(QObject *parent = nullptr);
  virtual ~SteamInput();

  Q_INVOKABLE
  void runFrame();

  IGA iga() const;

  const QString &igaPath() const;
  void setIgaPath(const QString &newIgaPath);

  QList<QSteamworks::Controller *> controllers() const;

  QSteamworks::Controller *lastController() const;

signals:
  void configurationLoaded();
  void igaChanged();
  void igaPathChanged();

  void controllersChanged();

  void lastControllerChanged();

private:
  IGA m_iga;
  QMap<InputHandle_t, QSteamworks::Controller *> m_controllers;
  static SteamInput *m_instance;
  QString m_igaPath;
  Controller *m_lastController = nullptr;

  static SteamInput *instance();
  void onActionEvent(SteamInputActionEvent_t *event);
};
} // namespace QSteamworks

Q_DECLARE_METATYPE(InputHandle_t)
