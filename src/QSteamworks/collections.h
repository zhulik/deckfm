#pragma once

#include <QList>

namespace QSteamworks {

template <typename T, typename L> T findBy(QList<T> list, L lambda) {
  foreach (auto &item, list) {
    if (lambda(item)) {
      return item;
    }
  }
  return T();
}
};
