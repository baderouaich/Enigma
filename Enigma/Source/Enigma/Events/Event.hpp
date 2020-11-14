#pragma once
#ifndef ENIGMA_EVENT_H
#define ENIGMA_EVENT_H

#include <Enigma/Core/Core.hpp>


NS_ENIGMA_BEGIN
enum class EventType
{
	NONE = 0,

	//WINDOW
	WINDOW_CLOSE,
	WINDOW_RESIZE,
	WINDOW_FOCUS,
	WINDOW_LOST_FOCUS,
	WINDOW_MOVED,
	WINDOW_MAXIMIZED,

	//FRAME BUFFER
	FRAME_BUFFER_RESIZE,

	//APPLICATION
	APP_TICK,
	APP_UPDATE,
	APP_RENDER,

	//KEYBOARD
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_TYPED,

	//MOUSE
	MOUSE_BUTTON_PRESSED,
	MOUSE_BUTTON_RELEASED,
	MOUSE_MOTION,
	MOUSE_WHEEL,

	//JOYSTICK
	JOYSTICK_CONNECTED,
	JOYSTICK_DISCONNECTED
};

enum EventCategory
{
	EC_NONE = 0,
	EC_APPLICATION = BIT(0),
	EC_INPUT = BIT(1),
	EC_KEYBOARD = BIT(2),
	EC_MOUSE = BIT(3),
	EC_MOUSE_BUTTON = BIT(4),
	EC_JOYSTICK = BIT(5)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class ENIGMA_API Event
{
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	const bool IsInCategory(EventCategory category) const noexcept
	{
		return GetCategoryFlags() & category;
	}

	const bool& IsHandled() const noexcept { return m_isHandled; }
	const void SetHandled(const bool handled) noexcept { m_isHandled = handled; }

protected:
	bool m_isHandled = false;
};



inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}
NS_ENIGMA_END

#endif // !ENIGMA_EVENT_H
