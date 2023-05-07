#include <QDebug>

#include "actiondefinition.h"
#include "actionsetdefinition.h"
#include "actionsetlayerdefinition.h"
#include "iga.h"
#include "qalgorithms.h"

using namespace QSteamworks;

static const QMap<QString, bool> actionTypes{{"Button", true}, {"StickPadGyro", false}, {"AnalogTrigger", false}};

IGA::IGA(const QJsonObject &definition) {
  auto manifest = definition["Action Manifest"].toObject();
  auto actions = manifest["actions"].toObject();

  foreach (auto &layer, manifest["action_layers"].toObject().toVariantMap().toStdMap()) {
    QList<ActionDefinition *> actions;
    foreach (auto &type, actionTypes.toStdMap()) {
      foreach (auto &name, layer.second.toMap()[type.first].toMap().keys()) {
        auto action = new ActionDefinition(name, type.first, layer.first, type.second);
        m_actions << action;
        actions << actions;
      }
    }
    m_actionSetLayers[layer.first] = new ActionSetLayerDefinition(layer.first, actions);
  }

  foreach (auto &actionSet, actions.toVariantMap().toStdMap()) {
    QList<ActionDefinition *> actions;
    foreach (auto &type, actionTypes.toStdMap()) {
      foreach (auto &name, actionSet.second.toMap()[type.first].toMap().keys()) {
        auto action = new ActionDefinition(name, type.first, actionSet.first, type.second);
        m_actions << action;
        actions << action;
      }
    }
    QList<ActionSetLayerDefinition *> layers;

    auto layerNames = actionSet.second.toMap()["Layers"];

    if (layerNames.isValid()) {
      foreach (auto &name, layerNames.toMap().keys()) {
        layers << m_actionSetLayers[name];
      }
    }

    m_actionSets[actionSet.first] = new ActionSetDefinition(actionSet.first, actions, layers);
  }
}

IGA::~IGA() {
  qDeleteAll(m_actionSets);
  qDeleteAll(m_actionSetLayers);
  qDeleteAll(m_actions);
}

const QMap<QString, ActionSetDefinition *> &IGA::actionSets() const { return m_actionSets; }

QStringList IGA::qmlActionSets() const { return m_actionSets.keys(); }

QStringList IGA::actionsForSet(const QString &name) const {
  QStringList result;
  foreach (auto &action, m_actionSets[name]->actions()) {
    result << action->name();
  }
  return result;
}

QVariantList IGA::qmlActionsForSet(const QString &name) const {
  QVariantList result;

  foreach (auto &action, m_actionSets[name]->actions()) {
    result << QVariant::fromValue(action);
  }
  return result;
}

QStringList IGA::qmlActions() const {
  QStringList result;
  foreach (auto &actionSet, m_actionSets.toStdMap()) {
    foreach (auto &action, actionSet.second->actions()) {
      result << action->name();
    }
  }
  return result;
}

QSteamworks::ActionDefinition *IGA::actionDefinition(const QString &name) const {
  foreach (auto &actionSet, m_actionSets.toStdMap()) {
    foreach (auto &action, actionSet.second->actions()) {
      if (action->name() == name) {
        return action;
      }
    }
  }
  return nullptr;
}

QStringList IGA::qmlActionSetLayers() const {
  QStringList result;
  foreach (auto &layer, m_actionSetLayers.toStdMap()) {
    result << layer.first;
  }
  return result;
}
