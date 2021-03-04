#pragma once
#ifndef ENIGMA_MOUSE_EVENT_H
#define ENIGMA_MOUSE_EVENT_H

#include <Core/Core.hpp>
#include <Core/Types.hpp>
#include <Input/Mouse/MouseCodes.hpp>
#include <sstream>

#include "Event.hpp"

NS_ENIGMA_BEGIN


class MouseMotionEvent : public Event
{
public:
	MouseMotionEvent(const f32 x, const f32 y)
		:
		m_mouseX(x),
		m_mouseY(y) 
	{}

	const f32& GetX() const noexcept { return m_mouseX; }
	const f32& GetY() const noexcept { return m_mouseY; }

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "MouseMotionEvent: " << m_mouseX << ", " << m_mouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MOUSE_MOTION)
	EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)

private:
	f32 m_mouseX, m_mouseY;
};

class MouseWheelEvent : public Event
{
public:
	MouseWheelEvent(const f32 xOffset, const f32 yOffset)
		:
		m_XOffset(xOffset),
		m_YOffset(yOffset)
	{}

	const f32& GetXOffset() const noexcept { return m_XOffset; }
	const f32& GetYOffset() const noexcept { return m_YOffset; }

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "MouseWheelEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MOUSE_WHEEL)
	EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)

private:
	f32 m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
	const MouseCode& GetMouseButton() const noexcept { return m_button; }

	virtual String ToString() const override = 0;

	EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)
protected:
	explicit MouseButtonEvent(const MouseCode button)
		:
		m_button(button) 
	{}

	MouseCode m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	explicit MouseButtonPressedEvent(const MouseCode button)
		:
		MouseButtonEvent(button) 
	{}

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_PRESSED)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	explicit MouseButtonReleasedEvent(const MouseCode button)
		: 
		MouseButtonEvent(button)
	{}

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_RELEASED)
};

NS_ENIGMA_END

#endif // !ENIGMA_MOUSE_EVENT_H
