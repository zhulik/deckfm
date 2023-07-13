#pragma once

#include <QNetworkAccessManager>
#include <QQmlNetworkAccessManagerFactory>

class CachedNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory {
  virtual QNetworkAccessManager *create(QObject *parent) override;
};

class CachedNetworkAccessManager : public QNetworkAccessManager {
  Q_OBJECT
public:
  CachedNetworkAccessManager(QObject *parent = nullptr);

  virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData) override;
};
