#include <QDir>
#include <QTimer>

#include "controller.h"
#include "inputevent.h"
#include "steam/isteaminput.h"

#include "collections.h"

using namespace QSteamworks;
using namespace QSteamworks::QSteamInput;

Controller::Controller(InputHandle_t handle, const QString &name, const IGA &iga, QObject *parent)
    : QObject(parent), m_handle(handle), m_name(name), m_iga(iga),
      m_image(QDir::current().absoluteFilePath("./resources/images/controllers/%1.png").arg(name)) {
  QTimer *timer = new QTimer();
  timer->start(500);
  connect(timer, &QTimer::timeout, timer, [timer, this]() {
    timer->deleteLater();
    loadActions();
  });
}

InputHandle_t Controller::handle() const { return m_handle; }

const QString &Controller::name() const { return m_name; }

const QString &Controller::image() const { return m_image; }

QList<Action> Controller::getActions(InputActionSetHandle_t actionSetHandle,
                                     const QList<ActionDefinition> &actions) const {
  QList<Action> result;

  foreach (auto &action, actions) {
    QStringList origins;
    QStringList glyphs;

    QVector<EInputActionOrigin> originsBuf(STEAM_INPUT_MAX_ORIGINS);

    InputHandle_t handle = 0;
    int n = 0;
    QString localizedName;

    if (action.isDigital()) {
      handle = ::SteamInput()->GetDigitalActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = ::SteamInput()->GetDigitalActionOrigins(m_handle, actionSetHandle, handle, originsBuf.data());

      localizedName = ::SteamInput()->GetStringForDigitalActionName(handle);
    } else {
      handle = ::SteamInput()->GetAnalogActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = ::SteamInput()->GetAnalogActionOrigins(m_handle, actionSetHandle, handle, originsBuf.data());
      localizedName = ::SteamInput()->GetStringForAnalogActionName(handle);
    }
    originsBuf.resize(n);

    foreach (auto &origin, originsBuf) {
      origins << ::SteamInput()->GetStringForActionOrigin(origin);
      glyphs << ::SteamInput()->GetGlyphSVGForActionOrigin(origin, 0);
    }

    result << Action(handle, action, localizedName, origins, glyphs);
  }
  return result;
}

QList<ActionSetLayer> Controller::getActionSetLayers(const QList<ActionSetLayerDefinition> &definitions) const {
  QList<ActionSetLayer> result;
  foreach (auto &definition, definitions) {
    auto handle = ::SteamInput()->GetActionSetHandle(definition.name().toLocal8Bit());
    result << ActionSetLayer(handle, definition.name(), getActions(handle, definition.actions()));
  }
  return result;
}

QList<QSteamworks::QSteamInput::ActionSet> Controller::actionSets() const { return m_actionSets.values(); }

void Controller::loadActions() {
  m_actionSets.clear();
  foreach (auto &actionSet, m_iga.actionSets()) {
    auto handle = ::SteamInput()->GetActionSetHandle(actionSet.name().toLocal8Bit());
    Q_ASSERT(handle != 0);
    m_actionSets[handle] = ActionSet(handle, actionSet.name(), getActions(handle, actionSet.actions()),
                                     getActionSetLayers(actionSet.layers()));
  }
  emit actionSetsChanged();
}

void Controller::showBindingPanel() const { ::SteamInput()->ShowBindingPanel(m_handle); }

void Controller::activateActionSetLayer(const ActionSetLayer &layer) {
  ::SteamInput()->ActivateActionSetLayer(m_handle, layer.handle());
  emit activeActionSetLayersChanged();
}

void Controller::deactivateActionSetLayer(const ActionSetLayer &layer) {
  ::SteamInput()->DeactivateActionSetLayer(m_handle, layer.handle());
  emit activeActionSetLayersChanged();
}

void Controller::deactivateAllActionSetLayers() {
  ::SteamInput()->DeactivateAllActionSetLayers(m_handle);
  emit activeActionSetLayersChanged();
}

ActionSet Controller::actionSetByName(const QString &name) {
  return findBy(m_actionSets.values(), [name](auto s) { return s.name() == name; });
}

void Controller::stopAnalogActionMomentum(const Action &action) {
  ::SteamInput()->StopAnalogActionMomentum(m_handle, action.handle());
}

void Controller::triggerRepeatedHapticPulse(unsigned short usDurationMicroSec, unsigned short usOffMicroSec,
                                            unsigned short unRepeat) {
  ::SteamInput()->Legacy_TriggerRepeatedHapticPulse(m_handle, k_ESteamControllerPad_Left, usDurationMicroSec,
                                                    usOffMicroSec, unRepeat, 0);
}

void Controller::setActionSet(const QSteamworks::QSteamInput::ActionSet &newActionSet) {
  if (m_actionSet == newActionSet) {
    return;
  }
  m_actionSet = newActionSet;
  ::SteamInput()->ActivateActionSet(m_handle, m_actionSet.handle());

  emit actionSetChanged();
}

QSteamworks::QSteamInput::ActionSet Controller::actionSet() const { return m_actionSet; }

void Controller::onActionEvent(SteamInputActionEvent_t *event) {
  InputHandle_t actionHandle = 0;
  bool digitalState = false;
  float analogX = 0;
  float analogY = 0;

  auto digital = event->eEventType == ESteamInputActionEventType_DigitalAction;

  emit userInteracted();

  if (digital) {
    actionHandle = event->digitalAction.actionHandle;

    digitalState = event->digitalAction.digitalActionData.bState;
  } else {
    actionHandle = event->analogAction.actionHandle;

    // TODO: add support for event->analogAction.analogActionData.eMode
    analogX = event->analogAction.analogActionData.x;
    analogY = event->analogAction.analogActionData.y;
  }

  auto a = m_actionSet.actionByHandle(actionHandle, event->eEventType == ESteamInputActionEventType_DigitalAction);

  if (!a.handle()) {
    return;
  }
  updateActionStates(a, digitalState, analogX, analogY);

  emit inputEvent(InputEvent(this, a, digitalState, analogX, analogY));
}

void Controller::updateActionStates(const Action &action, bool digitalState, float analogX, float analogY) {
  QVariant state;

  if (action.digital()) {
    state = digitalState;
  } else {
    state = QVariantMap{{"x", analogX}, {"y", analogY}};
  }

  m_actionStates[action.name()] = state;

  emit actionStatesChanged();
}

QVariantMap Controller::actionStates() const { return m_actionStates; }

QList<ActionSetLayer> Controller::activeActionSetLayers() const {
  QVector<InputActionSetHandle_t> buf(STEAM_INPUT_MAX_ACTIVE_LAYERS);
  auto n = ::SteamInput()->GetActiveActionSetLayers(m_handle, buf.data());
  buf.resize(n);

  QList<ActionSetLayer> result;

  foreach (auto &handle, buf) {
    auto l = findBy(m_actionSet.layers(), [handle](auto l) { return l.handle() == handle; });

    if (l.handle()) {
      result << l;
    }
  }

  return result;
}

void Controller::setActiveActionSetLayers(const QList<ActionSetLayer> &newActiveActionSetLayers) {
  deactivateAllActionSetLayers();

  foreach (auto &l, newActiveActionSetLayers) {
    ::SteamInput()->ActivateActionSetLayer(m_handle, l.handle());
  }

  emit activeActionSetLayersChanged();
}
