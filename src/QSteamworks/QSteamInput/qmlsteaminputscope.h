#pragma once

#include <QQuickItem>

namespace QSteamworks {

class SteamInput;
namespace QSteamInput {

class QMLSteamInputControl;

class QMLSteamInputScope : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QList<QSteamworks::QSteamInput::QMLSteamInputControl *> activeControls READ activeControls WRITE
                 setActiveControls NOTIFY activeControlsChanged)

public:
  QList<QMLSteamInputControl *> activeControls() const;

  virtual void componentComplete() override;

signals:
  void activeControlsChanged();

private:
  QSet<QMLSteamInputControl *> m_inputControls;
  QList<QMLSteamInputControl *> m_activeControls;
  SteamInput *m_steamInput;

  friend class QMLSteamInputControl;
  void addControl(QMLSteamInputControl *);

  void rebuildInputStack();
  void setActiveControls(const QList<QMLSteamInputControl *> &newActiveControls);
};

} // namespace QSteamInput
} // namespace QSteamworks
