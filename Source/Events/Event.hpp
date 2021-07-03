#pragma once
#ifndef ENIGMA_EVENT_H
#define ENIGMA_EVENT_H

#include <Core/Core.hpp>


NS_ENIGMA_BEGIN
enum class EventType : ui32
{
	NONE = 0,

	//WINDOW
	WINDOW_CLOSE,
	WINDOW_RESIZE,
	WINDOW_FOCUS,
	WINDOW_LOST_FOCUS,
	WINDOW_MOVED,
	WINDOW_MAXIMIZED,
	WINDOW_FILE_DROP,

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
inline ui32 operator |(const EventType& a, EventType b) { return static_cast<ui32>(a) | static_cast<ui32>(b); }
inline ui32 operator &(const EventType& a, EventType b) { return static_cast<ui32>(a) & static_cast<ui32>(b); }

enum class EventCategory : ui32
{
	NONE = 0,
	APPLICATION = BIT(0),
	INPUT = BIT(1),
	KEYBOARD = BIT(2),
	MOUSE = BIT(3),
	MOUSE_BUTTON = BIT(4),
	JOYSTICK = BIT(5)
};
inline ui32 operator |(const EventCategory& a, EventCategory b) { return static_cast<ui32>(a) | static_cast<ui32>(b); }
inline ui32 operator &(const EventCategory& a, EventCategory b) { return static_cast<ui32>(a) & static_cast<ui32>(b); }

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; } \
								virtual EventType GetEventType() const override { return GetStaticType(); } \
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual i32 GetCategoryFlags() const override { return static_cast<i32>(category); }

class ENIGMA_API Event
{
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	const bool IsInCategory(EventCategory category) const noexcept
	{
		return GetCategoryFlags() & static_cast<i32>(category);
	}

	const bool IsHandled() const noexcept { return m_isHandled; }
	const void SetHandled(const bool handled) noexcept { m_isHandled = handled; }

protected:
	bool m_isHandled{ false };
};



inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}
NS_ENIGMA_END

#endif // !ENIGMA_EVENT_H
