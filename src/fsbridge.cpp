#include <QFileInfo>
#include <QMimeDatabase>
#include <QUrl>
#include <QDir>
#include <QLocale>

#include "fsbridge.h"

FSBridge::FSBridge(QObject *parent)
    : QObject{parent}
{

}

QStringList FSBridge::mime(const QString &path) const
{
    return QMimeDatabase().mimeTypeForFile(path).name().split("/");
}

bool FSBridge::isReadable(const QString &path) const
{
    QFileInfo dir(pathFromURL(path));

    return dir.isReadable();
}

QString FSBridge::icon(const QString &path) const
{
    auto type = mime(path);
    auto category = type[0];
    auto name = type[1];

    if (category == "inode" && name == "directory") {
        return "folder";
    }

    if (category == "image") {
        return "fileImage";
    }

   if (category == "video") {
       return "fileVideo";
   }

   return "file";
}

QString FSBridge::humanReadableSize(const QString &path, bool includeHidden) const
{
    QFileInfo info(path);
    if (info.isDir()) {
        auto flags = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot;
        if (includeHidden) {
            flags = flags | QDir::Hidden;
        }
        auto files = QDir(path).entryList(flags);
        return QString("%1 Items").arg(files.count());
    }

    return QLocale().formattedDataSize(info.size());

}

QString FSBridge::pathFromURL(const QString &path) const
{
    return QUrl(path).path();
}

QVariantList FSBridge::pathToComponenets(const QString &path) const
{
    QVariantList result;
    auto parts = path.split(QDir::separator());

    while (parts.count() > 0) {
        auto full = parts.join(QDir::separator());

        auto part = parts.takeLast();

        if (part != "") {
            result.prepend(QVariantMap({
                                      {"name", part.replace(QDir::separator(), "")},
                                      {"path", full }
                                  }));
        }

    }
    return result;
}
