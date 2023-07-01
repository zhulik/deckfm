#include "inputevent.h"

using namespace QSteamworks;
using namespace QSteamworks::QSteamInput;

InputEvent::InputEvent(const QString &type, Controller *controller, const Action &action, bool digitalState,
                       float analogX, float analogY)
    : m_type(type), m_controller(controller), m_action(action), m_digitalState(digitalState), m_analogX(analogX),
      m_analogY(analogY) {}

const QString &InputEvent::type() const { return m_type; }

Controller *InputEvent::controller() const { return m_controller; }

Action InputEvent::action() const { return m_action; }

bool InputEvent::digitalState() const { return m_digitalState; }

float InputEvent::analogX() const { return m_analogX; }

float InputEvent::analogY() const { return m_analogY; }
