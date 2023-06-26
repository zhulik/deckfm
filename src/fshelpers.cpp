#include <QDir>
#include <QMimeDatabase>
#include <QUrl>

#include "fshelpers.h"

FSHelpers::FSHelpers(QObject *parent) : QObject{parent} {}

QString FSHelpers::mime(const QString &path) const { return QMimeDatabase().mimeTypeForFile(path).name(); }

QString FSHelpers::fileName(const QUrl &url) const { return url.path().split(QDir::separator()).last(); }
