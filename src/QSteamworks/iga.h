#pragma once

#include <QJsonObject>

namespace QSteamworks {

class IGA
{
    Q_GADGET
    Q_PROPERTY(QStringList actionSets READ actionSets CONSTANT)

public:
    IGA();
    IGA(const QJsonObject &definition);
    QStringList actionSets() const;

    Q_INVOKABLE
    QStringList actionsForSet(const QString&) const;

private:
    QMap<QString, QStringList> m_actionSets;
};
};

Q_DECLARE_METATYPE(QSteamworks::IGA)

