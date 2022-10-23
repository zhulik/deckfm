#include <QMimeDatabase>

#include "fshelpers.h"

FSHelpers::FSHelpers(QObject *parent) : QObject{parent} {}

QString FSHelpers::mime(const QString &path) const { return QMimeDatabase().mimeTypeForFile(path).name(); }
