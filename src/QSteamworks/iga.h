#pragma once

#include <QJsonObject>
#include <actiondefinition.h>

namespace QSteamworks {

class IGA
{
    Q_GADGET
    Q_PROPERTY(QStringList actionSets READ actionSets CONSTANT)
    Q_PROPERTY(QStringList actions READ actions CONSTANT)

public:
    IGA();
    IGA(const QJsonObject &definition);
    QStringList actionSets() const;

    Q_INVOKABLE
    QStringList actionsForSet(const QString&) const;

    Q_INVOKABLE
    QStringList actions() const;

private:
    QMap<QString, QList<ActionDefinition>> m_actionSets;
};
};

Q_DECLARE_METATYPE(QSteamworks::IGA)

