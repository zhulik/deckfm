#pragma once

#include <QObject>

#include "QSteamInput/iga.h"

#include "QSteamInput/inputevent.h"

namespace QSteamworks {

class SteamInput : public QObject {
  Q_OBJECT

  Q_PROPERTY(QSteamworks::QSteamInput::IGA iga READ iga NOTIFY igaChanged)
  Q_PROPERTY(QList<QSteamworks::QSteamInput::Controller *> controllers READ controllers NOTIFY controllersChanged)
  Q_PROPERTY(QString igaPath READ igaPath WRITE setIgaPath NOTIFY igaPathChanged)

  Q_PROPERTY(QSteamworks::QSteamInput::Controller *lastController READ lastController NOTIFY lastControllerChanged)

  STEAM_CALLBACK(SteamInput, onControllerConnected, SteamInputDeviceConnected_t);
  STEAM_CALLBACK(SteamInput, onControllerDisconnected, SteamInputDeviceDisconnected_t);
  STEAM_CALLBACK(SteamInput, onConfigurationLoaded, SteamInputConfigurationLoaded_t);

public:
  explicit SteamInput(QObject *parent = nullptr);
  virtual ~SteamInput();
  static SteamInput *instance();

  Q_INVOKABLE
  void runFrame();

  QSteamInput::IGA iga() const;

  const QString &igaPath() const;
  void setIgaPath(const QString &newIgaPath);

  QList<QSteamInput::Controller *> controllers() const;

  QSteamInput::Controller *lastController() const;
  void setLastController(QSteamInput::Controller *);

signals:
  void configurationLoaded();
  void igaChanged();
  void igaPathChanged();
  void controllersChanged();
  void lastControllerChanged();
  void inputEvent(const QSteamworks::QSteamInput::InputEvent &event);

private:
  QSteamInput::IGA m_iga;
  QMap<InputHandle_t, QSteamInput::Controller *> m_controllers;
  static SteamInput *m_instance;
  QString m_igaPath;
  QSteamInput::Controller *m_lastController = nullptr;

  void onActionEvent(SteamInputActionEvent_t *event);
};
} // namespace QSteamworks

Q_DECLARE_METATYPE(InputHandle_t)
