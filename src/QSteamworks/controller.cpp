#include <QDir>

#include "controller.h"

using namespace QSteamworks;

Controller::Controller(InputHandle_t handle, const QString &name, const IGA &iga, QObject *parent)
    : QObject(parent), m_handle(handle), m_name(name), m_iga(iga),
      m_image(QDir::current().absoluteFilePath("./resources/images/controllers/%1.png").arg(name)) {
  loadActions();
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
      handle = SteamInput()->GetDigitalActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = SteamInput()->GetDigitalActionOrigins(m_handle, actionSetHandle, handle, originsBuf.data());

      localizedName = SteamInput()->GetStringForDigitalActionName(handle);
    } else {
      handle = SteamInput()->GetAnalogActionHandle(action.name().toLocal8Bit());
      Q_ASSERT(handle != 0);

      n = SteamInput()->GetAnalogActionOrigins(m_handle, actionSetHandle, handle, originsBuf.data());
      localizedName = SteamInput()->GetStringForAnalogActionName(handle);
    }
    originsBuf.resize(n);

    foreach (auto &origin, originsBuf) {
      origins << SteamInput()->GetStringForActionOrigin(origin);
      glyphs << SteamInput()->GetGlyphSVGForActionOrigin(origin, 0);
    }

    result << Action(handle, action, localizedName, origins, glyphs);
  }
  return result;
}

QList<ActionSetLayer> Controller::getActionSetLayers(const QList<ActionSetLayerDefinition> &definitions) const {
  QList<ActionSetLayer> result;
  foreach (auto &definition, definitions) {
    auto handle = SteamInput()->GetActionSetHandle(definition.name().toLocal8Bit());
    result << ActionSetLayer(handle, definition.name(), getActions(handle, definition.actions()));
  }
  return result;
}

QList<QSteamworks::ActionSet> Controller::actionSets() const { return m_actionSets; }

void Controller::loadActions() {
  m_actionSets.clear();
  foreach (auto &actionSet, m_iga.actionSets()) {
    auto handle = SteamInput()->GetActionSetHandle(actionSet.name().toLocal8Bit());
    Q_ASSERT(handle != 0);
    m_actionSets << ActionSet(handle, actionSet.name(), getActions(handle, actionSet.actions()),
                              getActionSetLayers(actionSet.layers()));
  }
  emit actionsSetsChanged();
}
