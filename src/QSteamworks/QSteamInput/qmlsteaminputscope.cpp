#include "qmlsteaminputscope.h"
#include "QSteamInput/qmlsteaminputcontrol.h"
#include "qdebug.h"
#include "qquickitem.h"
#include "steaminput.h"

using namespace QSteamworks;
using namespace QSteamworks::QSteamInput;

void QMLSteamInputScope::addControl(QMLSteamInputControl *control) {
  m_inputControls << control;

  connect(control, &QMLSteamInputControl::destroyed, this, [this, control]() {
    m_inputControls.remove(control);
    rebuildInputStack();
  });

  connect(control, &QMLSteamInputControl::enabledChanged, this, &QMLSteamInputScope::rebuildInputStack);
  rebuildInputStack();
}

static QList<QPair<long int, QMLSteamInputControl *>> buildStack(QQuickItem *root, long int level = 0) {
  QList<QPair<long int, QMLSteamInputControl *>> stack;

  if (auto control = qobject_cast<QMLSteamInputControl *>(root)) {
    if (control->isEnabled()) {
      stack << QPair{level, control};
    }
  }

  foreach (auto child, root->children()) {
    auto item = qobject_cast<QQuickItem *>(child);
    if (item) {
      stack += buildStack(qobject_cast<QQuickItem *>(child), level + 1);
    }
  }

  return stack;
}

void QMLSteamInputScope::rebuildInputStack() {
  auto pairs = buildStack(this);
  QVector<QList<QMLSteamInputControl *>> stack(pairs.count());

  QList<QMLSteamInputControl *> globals;

  auto i = 0;
  foreach (auto pair, pairs) {
    if (pair.second->global()) {
      globals << pair.second;
      stack.resize(stack.size() - 1);
      continue;
    }

    stack[i] << pair.second;
    i++;
  }

  setActiveControls(stack.last() + globals);
}

QList<QSteamworks::QSteamInput::QMLSteamInputControl *> QMLSteamInputScope::activeControls() const {
  return m_activeControls;
}

void QMLSteamInputScope::componentComplete() {
  QQuickItem::componentComplete();
  m_steamInput = SteamInput::instance();

  connect(m_steamInput, &SteamInput::inputEvent, this, [this](auto e) {
    foreach (auto control, m_activeControls) {
      if (control->controller() == e.controller()) {
        control->onInputEvent(e);
      }
    }
  });
}

void QMLSteamInputScope::setActiveControls(const QList<QMLSteamInputControl *> &newActiveControls) {
  if (m_activeControls == newActiveControls)
    return;
  m_activeControls = newActiveControls;

  emit activeControlsChanged();
}
