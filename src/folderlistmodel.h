#pragma once

#include "qobjectdefs.h"
#include <QAbstractListModel>

enum FileType { Directory, File };

struct FileInfo {
  bool isDir;
  QString name;
  QString path;
  QString mime;
  QString icon;
  QString sizeString;

  bool readable;
  bool writeable;
};

class FolderListModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged);
  Q_PROPERTY(int count READ count NOTIFY countChanged);
  Q_PROPERTY(bool canGoUp READ canGoUp NOTIFY canGoUpChanged);
  Q_PROPERTY(QVariantList pathComponents READ pathComponents NOTIFY pathComponentsChanged)
  Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)

  enum Role { IsDir = Qt::UserRole, Name, Path, Mime, Icon, SizeString, Readable, Writeable };

public:
  explicit FolderListModel(QObject *parent = nullptr);

  Q_INVOKABLE void goUp();
  Q_INVOKABLE void goHome();
  Q_INVOKABLE QVariantMap get(int index) const;

  virtual int rowCount(const QModelIndex &parent) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
  virtual QHash<int, QByteArray> roleNames() const override;

  const QString &path() const;
  void setPath(const QString &newPath);

  int count() const;
  bool canGoUp() const;

  QVariantList pathComponents() const;

  bool showHidden() const;
  void setShowHidden(bool newShowHidden);

signals:
  void pathChanged(const QString &);
  void countChanged(int);
  void canGoUpChanged(bool);
  void pathComponentsChanged(QVariantList);

  void showHiddenChanged(bool);

private:
  QList<FileInfo> m_folderContent;
  QString m_path;

  void updateContent();
  bool m_showHidden;

  QString sizeString(const QString &) const;
};
