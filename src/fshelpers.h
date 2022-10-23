#pragma once

#include "qobjectdefs.h"
#include <QObject>

class FSHelpers : public QObject {
  Q_OBJECT
public:
  explicit FSHelpers(QObject *parent = nullptr);

  Q_INVOKABLE
  QString mime(const QString &path) const;

signals:
};
