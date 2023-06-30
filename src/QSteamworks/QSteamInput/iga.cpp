#include "iga.h"
#include "actiondefinition.h"
#include "actionsetdefinition.h"
#include "actionsetlayerdefinition.h"

using namespace QSteamworks;

static const QMap<QString, bool> actionTypes{{"Button", true}, {"StickPadGyro", false}, {"AnalogTrigger", false}};

IGA::IGA(const QJsonObject &definition) {
  QMap<QString, ActionSetLayerDefinition> actionSetLayers;

  auto manifest = definition["Action Manifest"].toObject();
  auto actionSets = manifest["actions"].toObject();
  auto actionLayers = manifest["action_layers"].toObject();

  foreach (auto &layerName, actionLayers.keys()) {
    auto layer = actionLayers[layerName].toObject();
    QList<ActionDefinition> actions;

    foreach (auto &actionTypeName, actionTypes.keys()) {
      foreach (auto &actionName, layer[actionTypeName].toObject().keys()) {
        actions << ActionDefinition(actionName, actionTypeName, layerName, actionTypes[actionTypeName]);
      }
    }
    actionSetLayers[layerName] = ActionSetLayerDefinition(layerName, actions);
  }

  foreach (auto &actionSetName, actionSets.keys()) {
    auto actionSet = actionSets[actionSetName].toObject();

    QList<ActionDefinition> actions;
    foreach (auto &actionTypeName, actionTypes.keys()) {
      foreach (auto &actionName, actionSet[actionTypeName].toObject().keys()) {
        actions << ActionDefinition(actionName, actionTypeName, actionSetName, actionTypes[actionTypeName]);
      }
    }
    QList<ActionSetLayerDefinition> layers;

    auto layerNames = actionSet["Layers"].toObject();

    foreach (auto &name, layerNames.keys()) {
      layers << actionSetLayers[name];
    }

    m_actionSets << ActionSetDefinition(actionSetName, actions, layers);
  }
}

QList<ActionSetDefinition> IGA::actionSets() const { return m_actionSets; }
