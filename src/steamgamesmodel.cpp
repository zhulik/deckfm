#include "steamgamesmodel.h"
#include "QSteamInput/vdfparser.h"
#include "qglobal.h"
#include "steam/steamtypes.h"

#include <QDebug>
#include <QStandardPaths>

static QString readFile(QString const &path) {
  QFile f(path);

  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    throw std::runtime_error(QString("Cannot read file %1").arg(path).toLocal8Bit());
  }

  return QTextStream(&f).readAll();
}

SteamGamesModel::SteamGamesModel(QObject *parent) : QAbstractListModel{parent} {
  m_root = QDir(QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0]);
  m_root.cd(".local/share/Steam");

  refresh();
}

int SteamGamesModel::rowCount(const QModelIndex &parent) const { return m_games.count(); }

QVariant SteamGamesModel::data(const QModelIndex &index, int role) const {
  auto game = m_games.values()[index.row()];
  switch (role) {
  case Id:
    return game.id;
  case Name:
    return game.name;
  default:
    return QVariant();
  }
}

QHash<int, QByteArray> SteamGamesModel::roleNames() const {
  return QHash<int, QByteArray>{{SteamGamesModel::Id, "id"}, {SteamGamesModel::Name, "name"}};
}

void SteamGamesModel::refresh() {
  VDFParser parser;

  auto libraryFolders =
      parser.parse(readFile(m_root.absoluteFilePath("config/libraryfolders.vdf")))["libraryfolders"].toObject();

  beginResetModel();
  m_games.clear();
  foreach (auto k, libraryFolders.keys()) {
    auto folderObj = libraryFolders[k].toObject();
    auto dir = QDir(folderObj["path"].toString());
    dir.cd("steamapps");
    Q_ASSERT(dir.exists());

    foreach (auto gameId, folderObj["apps"].toObject().keys()) {
      auto id = gameId.toUInt();
      auto manifest = parser.parse(readFile(dir.absoluteFilePath(QString("appmanifest_%1.acf").arg(gameId))));
      m_games[id] = SteamGame{id, manifest["AppState"].toObject()["name"].toString()};
    }
  }
  endResetModel();
}
