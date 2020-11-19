#pragma once
#ifndef ENIGMA_JOYSTICK_EVENT_H
#define ENIGMA_JOYSTICK_EVENT_H

#include <Core/Core.hpp>
#include <Core/Types.hpp>
#include <sstream>

#include "Event.hpp"

NS_ENIGMA_BEGIN


class JoystickConnectedEvent : public Event
{
public:
	explicit JoystickConnectedEvent(i32 jid)
		:
		m_joystick_id(jid)
	{}

	const i32& GetJoystickID() const noexcept { return m_joystick_id; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "Joystick Connected, ID: " << m_joystick_id;
		return ss.str();
	}

	EVENT_CLASS_TYPE(JOYSTICK_CONNECTED);
	EVENT_CLASS_CATEGORY(EC_JOYSTICK);
private:
	i32 m_joystick_id;
};

class JoystickDisconnectedEvent : public Event
{
public:
	explicit JoystickDisconnectedEvent(i32 jid)
		:
		m_joystick_id(jid)
	{}

	const i32& GetJoystickID() const noexcept { return m_joystick_id; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "Joystick Disconnected, ID: " << m_joystick_id;
		return ss.str();
	}

	EVENT_CLASS_TYPE(JOYSTICK_DISCONNECTED);
	EVENT_CLASS_CATEGORY(EC_JOYSTICK);
private:
	i32 m_joystick_id;
};


NS_ENIGMA_END

#endif // !ENIGMA_JOYSTICK_EVENT_H
