#include "qmlsteaminputcontrol.h"

#include <QQmlEngine>

#include "QSteamInput/actionsetlayer.h"
#include "QSteamInput/qmlsteaminputscope.h"
#include "QSteamworks/QSteamInput/controller.h"
#include "QSteamworks/QSteamInput/qmlsteaminputscope.h"

#include <stdexcept>

using namespace QSteamworks::QSteamInput;

Controller *QMLSteamInputControl::controller() const { return m_controller; }

void QMLSteamInputControl::setController(Controller *newController) {
  if (m_controller == newController)
    return;

  m_controller = newController;
  emit controllerChanged();

  if (!m_controller)
    return;

  applyActionSetAndLayers(); // TODO: fixme
}

QString QMLSteamInputControl::actionSet() const { return m_actionSet; }

void QMLSteamInputControl::setActionSet(const QString &newActionSet) {
  if (m_global) {
    qWarning() << "Global SteamInputControl cannot define an action set.";
    return;
  }

  if (m_actionSet == newActionSet)
    return;

  m_actionSet = newActionSet;

  if (m_controller) {
    m_controller->setActionSet(m_controller->actionSetByName(m_actionSet));
  }

  emit actionSetChanged();
}

QStringList QMLSteamInputControl::actionSetLayers() const { return m_actionSetLayers; }

void QMLSteamInputControl::setActionSetLayers(const QStringList &newActionSetLayers) {
  if (m_global) {
    qWarning() << "Global SteamInputControl cannot define set layers.";
    return;
  }

  if (m_actionSetLayers == newActionSetLayers)
    return;

  m_actionSetLayers = newActionSetLayers;

  activateActionSetLayers();
  emit actionSetChangedLayers();
}

void QMLSteamInputControl::componentComplete() {
  QQuickItem::componentComplete();

  auto currentParent = this->parent();
  while (currentParent != nullptr) {
    if (auto scope = qobject_cast<QMLSteamInputScope *>(currentParent)) {
      setScope(scope);
      return;
    }
    currentParent = currentParent->parent();
  }
  throw std::runtime_error("SteamInputControl must have a SteamInputScope among it's ancestors");
}

void QMLSteamInputControl::setScope(QMLSteamInputScope *scope) {
  if (m_scope == scope)
    return;

  m_scope = scope;

  scope->addControl(this);
}

QJSValue QMLSteamInputControl::handlerFor(const InputEvent &e) const {
  if (!e.digital()) {
    return m_analogHandlers.property(e.actionName());
  }

  if (e.digitalState()) {
    return m_pressHandlers.property(e.actionName());
  } else {
    return m_releaseHandlers.property(e.actionName());
  }
}

void QMLSteamInputControl::activateActionSetLayers() {
  if (!m_controller || m_global)
    return;

  QList<ActionSetLayer> layers;

  foreach (auto l, m_actionSetLayers) {
    layers << m_controller->actionSet().layerByName(l);
  }
  m_controller->setActiveActionSetLayers(layers);
}

void QMLSteamInputControl::onInputEvent(const InputEvent &e) {
  emit inputEvent(e);

  if (!e.digital()) {
    emit analogEvent(e);
  } else {
    if (e.digitalState()) {
      emit pressedEvent(e);
    } else {
      emit releasedEvent(e);
    }
  }

  auto handler = handlerFor(e);
  if (handler.isUndefined()) {
    return;
  }
  if (handler.isCallable()) {
    handler.call(QJSValueList{qmlEngine(this)->toScriptValue(e)});
  } else {
    qWarning() << "handler must be a function";
  }
}

QJSValue QMLSteamInputControl::pressHandlers() const { return m_pressHandlers; }

void QMLSteamInputControl::setPressHandlers(const QJSValue &newPressHandlers) {
  m_pressHandlers = newPressHandlers;
  emit pressHandlersChanged();
}

QJSValue QMLSteamInputControl::releaseHandlers() const { return m_releaseHandlers; }

void QMLSteamInputControl::setReleaseHandlers(const QJSValue &newReleaseHandlers) {
  m_releaseHandlers = newReleaseHandlers;
  emit releaseHandlersChanged();
}

QJSValue QMLSteamInputControl::analogHandlers() const { return m_analogHandlers; }

void QMLSteamInputControl::setAnalogHandlers(const QJSValue &newAnalogHandlers) {
  m_analogHandlers = newAnalogHandlers;
  emit analogHandlersChanged();
}

bool QMLSteamInputControl::global() const { return m_global; }

void QMLSteamInputControl::setglobal(bool newGlobal) {
  if (m_global == newGlobal)
    return;
  m_global = newGlobal;
  emit globalChanged();
}

void QMLSteamInputControl::applyActionSetAndLayers() {
  if (!isEnabled() || !m_controller)
    return;

  if (!m_actionSet.isEmpty()) {
    m_controller->setActionSet(m_controller->actionSetByName(m_actionSet));
  }

  activateActionSetLayers();
}
