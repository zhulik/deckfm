#pragma once

#include "qobjectdefs.h"
#include <QAbstractListModel>
#include <QDir>

struct SteamGame {
  uint id = -1;
  QString name;
  QString libraryImagePath;
};

class SteamGamesModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum Role { Id = Qt::UserRole, Name, LibraryImagePath };

  explicit SteamGamesModel(QObject *parent = nullptr);

  Q_INVOKABLE void refresh();

  virtual int rowCount(const QModelIndex &parent) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
  virtual QHash<int, QByteArray> roleNames() const override;

private:
  QDir m_root;
  QHash<uint, SteamGame> m_games;
};
