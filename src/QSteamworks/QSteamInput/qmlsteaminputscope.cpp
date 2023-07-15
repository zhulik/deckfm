#include "qmlsteaminputscope.h"
#include "QSteamInput/qmlsteaminputcontrol.h"
#include "steaminput.h"
#include <algorithm>

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

static QList<QPair<long int, QMLSteamInputControl *>> buildStack(QObject *root, long int level = 0) {
  QList<QPair<long int, QMLSteamInputControl *>> stack;

  if (root == nullptr) {
    return stack;
  }

  if (auto control = qobject_cast<QMLSteamInputControl *>(root)) {
    if (control->isEnabled()) {
      stack << QPair{level + control->z(), control};
    }
  }

  foreach (auto child, root->children()) {
    stack += buildStack(child, level + 1);
  }

  std::sort(stack.begin(), stack.end(), [](auto a, auto b) { return b.first > a.first; });

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

  if (stack.count() > 0) {
    setActiveControls(stack.last() + globals);
  } else {
    setActiveControls(globals);
  }
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
