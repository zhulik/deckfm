#include "controller.h"

using namespace QSteamworks;

Controller::Controller(InputHandle_t handle, const QString &name, QObject *parent)
    : QObject(parent), m_handle(handle), m_name(name),
      m_image(QString("resources/images/controllers/%1.png").arg(name)) {}

InputHandle_t Controller::handle() const { return m_handle; }

const QString &Controller::name() const { return m_name; }

const QString &Controller::image() const { return m_image; }

bool Controller::operator==(const Controller &other) const { return m_handle == other.handle(); }
