#include <QDir>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <stdexcept>

#include "folderlistmodel.h"
#include "qabstractitemmodel.h"
#include "qfileinfo.h"
#include "qnamespace.h"

static QMap<QString, QString> extensionIcons{{"qml", "application"}};

QString iconByExtension(const QString &extension) {
  if (extensionIcons.contains(extension)) {
    return extensionIcons[extension];
  }
  return "file";
}

QString icon(const QFileInfo &info, const QString &mime) {
  auto type = mime.split("/");
  auto category = type[0];
  auto name = type[1];

  if (category == "inode" && name == "directory") {
    return "folder";
  }

  if (category == "text" && name == "plain") {
    return iconByExtension(info.completeSuffix());
  }

  if (category == "text" && name == "x-qml") {
    return "application";
  }

  if (category == "image") {
    return "fileImage";
  }

  if (category == "video") {
    return "fileVideo";
  }

  return "file";
}

FolderListModel::FolderListModel(QObject *parent) : QAbstractListModel{parent} { goHome(); }

void FolderListModel::goUp() {
  QDir dir(m_path);
  dir.cdUp();
  setPath(dir.path());
}

void FolderListModel::goHome() { setPath(QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0]); }

QVariantMap FolderListModel::get(int index) const {
  auto item = m_folderContent[index];
  return QVariantMap{
      {"isDir", item.isDir},       {"name", item.name},           {"path", item.path},
      {"mime", item.mime},         {"icon", item.icon},           {"sizeString", item.sizeString},
      {"readable", item.readable}, {"writeable", item.writeable},
  };
}

int FolderListModel::rowCount(const QModelIndex &parent) const { return m_folderContent.count(); }

QVariant FolderListModel::data(const QModelIndex &index, int role) const {
  auto roleE = (FolderListModel::Role)role;
  auto item = m_folderContent[index.row()];

  switch (roleE) {
  case IsDir:
    return item.isDir;
    break;
  case Name:
    return item.name;
    break;
  case Path:
    return item.path;
    break;
  case Mime:
    return item.mime;
    break;
  case Icon:
    return item.icon;
    break;
  case SizeString:
    return item.sizeString;
    break;
  case Readable:
    return item.readable;
    break;
  case Writeable:
    return item.writeable;
    break;

  default:
    return "";
  }
}

QHash<int, QByteArray> FolderListModel::roleNames() const {
  return QHash<int, QByteArray>{
      {FolderListModel::IsDir, "fileIsDir"},       {FolderListModel::Name, "fileName"},
      {FolderListModel::Path, "filePath"},         {FolderListModel::Mime, "fileMime"},
      {FolderListModel::Icon, "fileIcon"},         {FolderListModel::SizeString, "fileSizeString"},
      {FolderListModel::Readable, "fileReadable"}, {FolderListModel::Writeable, "fileWriteable"}};
}

const QString &FolderListModel::path() const { return m_path; }

void FolderListModel::setPath(const QString &newPath) {
  if (m_path == newPath)
    return;
  m_path = newPath;
  emit pathChanged(m_path);
  emit pathComponentsChanged(pathComponents());
  updateContent();
}

int FolderListModel::count() const { return m_folderContent.count(); }

bool FolderListModel::canGoUp() const { return QDir(m_path).cdUp(); }

QVariantList FolderListModel::pathComponents() const {
  QVariantList result;
  auto parts = m_path.split(QDir::separator());

  while (parts.count() > 0) {
    auto full = parts.join(QDir::separator());

    auto part = parts.takeLast();

    if (part != "") {
      result.prepend(QVariantMap({{"name", part.replace(QDir::separator(), "")}, {"path", full}}));
    }
  }
  return result;
}

void FolderListModel::updateContent() {
  beginResetModel();
  m_folderContent.clear();

  auto flags = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot;
  if (m_showHidden) {
    flags = flags | QDir::Hidden;
  }

  foreach (auto &info, QDir(m_path).entryInfoList(flags, QDir::Name | QDir::DirsFirst)) {
    auto mime = QMimeDatabase().mimeTypeForFile(info.filePath()).name();
    m_folderContent << FileInfo{
        info.isDir(),      info.fileName(),  info.filePath(), mime, icon(info, mime), sizeString(info.filePath()),
        info.isReadable(), info.isWritable()};
  }
  endResetModel();
  emit countChanged(count());
  emit canGoUpChanged(canGoUp());
}

QString FolderListModel::sizeString(const QString &path) const {
  QFileInfo info(path);
  if (info.isDir()) {
    auto flags = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot;
    if (m_showHidden) {
      flags = flags | QDir::Hidden;
    }
    auto files = QDir(path).entryList(flags);
    return QString("%1 Items").arg(files.count());
  }

  return QLocale().formattedDataSize(info.size());
}

bool FolderListModel::showHidden() const { return m_showHidden; }

void FolderListModel::setShowHidden(bool newShowHidden) {
  if (m_showHidden == newShowHidden)
    return;
  m_showHidden = newShowHidden;

  emit showHiddenChanged(m_showHidden);
  updateContent();
}
