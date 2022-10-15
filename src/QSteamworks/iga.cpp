#include <QDebug>

#include "iga.h"

IGA::IGA()
{

}

IGA::IGA(const QJsonObject &definition)
{
    auto actions = definition["Action Manifest"].toObject()["actions"].toObject();
    qDebug() << actions;
//    auto actionLayers = definition["Action Manifest"].toObject()["action_layers"].toObject();
}
