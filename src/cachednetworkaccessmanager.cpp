#include "cachednetworkaccessmanager.h"

#include <QDir>
#include <QNetworkDiskCache>
#include <QStandardPaths>

CachedNetworkAccessManager::CachedNetworkAccessManager(QObject *parent) : QNetworkAccessManager{parent} {
  auto diskCache = new QNetworkDiskCache(this);

  auto root = QDir(QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0]);
  root.mkpath("network");
  root.cd("network");

  diskCache->setCacheDirectory(root.absolutePath());
  setCache(diskCache);
}

QNetworkReply *CachedNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &req,
                                                         QIODevice *outgoingData) {
  QNetworkRequest request(req);
  request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
  return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

QNetworkAccessManager *CachedNetworkAccessManagerFactory::create(QObject *parent) {
  return new CachedNetworkAccessManager(parent);
}
