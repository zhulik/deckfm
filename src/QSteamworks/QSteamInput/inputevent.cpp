#include "inputevent.h"

using namespace QSteamworks::QSteamInput;

InputEvent::InputEvent(Controller *controller, const Action &action, bool digitalState, float analogX, float analogY)
    : m_controller(controller), m_action(action), m_digitalState(digitalState), m_analogX(analogX), m_analogY(analogY) {
}

Controller *InputEvent::controller() const { return m_controller; }

const QString &InputEvent::actionName() const { return m_action.name(); }

Action InputEvent::action() const { return m_action; }

bool InputEvent::digitalState() const { return m_digitalState; }

float InputEvent::analogX() const { return m_analogX; }

float InputEvent::analogY() const { return m_analogY; }

bool InputEvent::digital() const { return m_action.digital(); }
