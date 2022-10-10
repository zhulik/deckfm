#pragma once

#include <QObject>
#include <QVariant>

class FSBridge : public QObject
{
    Q_OBJECT
public:
    explicit FSBridge(QObject *parent = nullptr);

    Q_INVOKABLE
    QStringList mime(const QString &path) const;

    Q_INVOKABLE
    bool isReadable(const QString &path) const;

    Q_INVOKABLE
    QString icon(const QString &path) const;

    Q_INVOKABLE
    QString humanReadableSize(const QString &path, bool includeHidden) const;

    Q_INVOKABLE
    QString pathFromURL(const QString &path) const;

    Q_INVOKABLE
    QVariantList pathToComponenets(const QString &path) const;

};
