#include <QDebug>

#include "iga.h"

using namespace QSteamworks;

static const QStringList actionTypes {
    "Button",
    "StickPadGyro"
};

IGA::IGA()
{

}

IGA::IGA(const QJsonObject &definition)
{
    auto actions = definition["Action Manifest"].toObject()["actions"].toObject();

    foreach(auto &actionSet, actions.toVariantMap().toStdMap()) {

        foreach(auto &type, actionTypes) {
            m_actionSets[actionSet.first].append(actionSet.second.toMap()[type].toMap().keys());
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
    return m_actionSets[name];
}
