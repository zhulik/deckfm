#pragma once

#include <QObject>
#include <QQuickItem>

#include "QSteamworks/QSteamInput/inputevent.h"

namespace QSteamworks {
namespace QSteamInput {

class Controller;
class QMLSteamInputScope;

class QMLSteamInputControl : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(bool global READ global WRITE setglobal NOTIFY globalChanged)

  Q_PROPERTY(
      QSteamworks::QSteamInput::Controller *controller READ controller WRITE setController NOTIFY controllerChanged)
  Q_PROPERTY(QString actionSet READ actionSet WRITE setActionSet NOTIFY actionSetChanged)
  Q_PROPERTY(QStringList actionSetLayers READ actionSetLayers WRITE setActionSetLayers NOTIFY actionSetChangedLayers)

  Q_PROPERTY(QJSValue pressHandlers READ pressHandlers WRITE setPressHandlers NOTIFY pressHandlersChanged)
  Q_PROPERTY(QJSValue releaseHandlers READ releaseHandlers WRITE setReleaseHandlers NOTIFY releaseHandlersChanged)
  Q_PROPERTY(QJSValue analogHandlers READ analogHandlers WRITE setAnalogHandlers NOTIFY analogHandlersChanged)

public:
  Controller *controller() const;
  void setController(Controller *newController);

  QString actionSet() const;
  void setActionSet(const QString &newActionSet);

  QStringList actionSetLayers() const;
  void setActionSetLayers(const QStringList &newActionSetLayers);

  virtual void componentComplete() override;

  QJSValue pressHandlers() const;
  void setPressHandlers(const QJSValue &newPressHandlers);

  QJSValue releaseHandlers() const;
  void setReleaseHandlers(const QJSValue &newReleaseHandlers);

  QJSValue analogHandlers() const;
  void setAnalogHandlers(const QJSValue &newAnalogHandlers);

  bool global() const;
  void setglobal(bool newGlobal);

  void applyActionSetAndLayers();

signals:
  void controllerChanged();
  void actionSetChanged();
  void actionSetChangedLayers();

  void inputEvent(const QSteamworks::QSteamInput::InputEvent &event);
  void analogEvent(const QSteamworks::QSteamInput::InputEvent &event);
  void pressedEvent(const QSteamworks::QSteamInput::InputEvent &event);
  void releasedEvent(const QSteamworks::QSteamInput::InputEvent &event);

  void pressHandlersChanged();
  void releaseHandlersChanged();
  void analogHandlersChanged();

  void globalChanged();

private:
  Controller *m_controller = nullptr;
  QString m_actionSet;
  QStringList m_actionSetLayers;
  QMLSteamInputScope *m_scope;

  QJSValue m_pressHandlers;
  QJSValue m_releaseHandlers;
  QJSValue m_analogHandlers;

  void setScope(QMLSteamInputScope *);

  friend class QMLSteamInputScope;
  void onInputEvent(const InputEvent &);

  QJSValue handlerFor(const InputEvent &e) const;
  bool m_global = false;

  void activateActionSetLayers();
};
} // namespace QSteamInput
} // namespace QSteamworks
