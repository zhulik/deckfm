#pragma once

#include <QObject>
#include <QVariant>

class FSBridge : public QObject
{
    Q_OBJECT
public:
    explicit FSBridge(QObject *parent = nullptr);

    Q_INVOKABLE
    QStringList mime(const QString &path);

    Q_INVOKABLE
    bool isReadable(const QString &path);

    Q_INVOKABLE
    QString icon(const QString &path);

    Q_INVOKABLE
    QString humanReadableSize(const QString &path, bool includeHidden);

    Q_INVOKABLE
    QString pathFromURL(const QString &path);

    Q_INVOKABLE
    QVariantList pathToComponenets(const QString &path);

};
