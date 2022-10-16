#include <QDebug>

#include "iga.h"

using namespace QSteamworks;

static const QMap<QString, bool> actionTypes {
    {"Button", false},
    {"StickPadGyro", true}
};

IGA::IGA()
{

}

IGA::IGA(const QJsonObject &definition)
{
    auto actions = definition["Action Manifest"].toObject()["actions"].toObject();

    foreach(auto &actionSet, actions.toVariantMap().toStdMap()) {

        foreach(auto &type, actionTypes.toStdMap()) {
            foreach(auto &name, actionSet.second.toMap()[type.first].toMap().keys()) {
                m_actionSets[actionSet.first].append(ActionDefinition(name, type.first, actionSet.first, type.second));
            }

        }
    }
    //    auto actionLayers = definition["Action Manifest"].toObject()["action_layers"].toObject();
}

QStringList IGA::actionSets() const
{
    return m_actionSets.keys();
}

QStringList IGA::actionsForSet(const QString &name) const
{
    QStringList result;
    foreach(auto &action, m_actionSets[name]) {
        result << action.name();
    }
    return result;
}

QStringList IGA::actions() const
{
    QStringList result;
    foreach(auto &actionSet, m_actionSets.toStdMap()) {
        foreach(auto &action, actionSet.second) {
            result << action.name();
        }
    }
    return result;
}

QSteamworks::ActionDefinition IGA::actionDefinition(const QString &name) const
{
    foreach(auto &actionSet, m_actionSets.toStdMap()) {
        foreach(auto &action, actionSet.second) {
            if (action.name() == name) {
                return action;
            }
        }
    }
    return ActionDefinition();
}
