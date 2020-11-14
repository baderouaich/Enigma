#pragma once
#ifndef ENIGMA_APPLICATION_EVENT_H
#define ENIGMA_APPLICATION_EVENT_H

#include <Enigma/Core/Core.hpp>
#include <Enigma/Core/Types.hpp>
#include <sstream>

#include "Event.hpp"

NS_ENIGMA_BEGIN

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(const i32 width, const i32 height)
		:
		m_width(width),
		m_height(height) {}

	const i32& GetWidth()  const noexcept { return m_width; }
	const i32& GetHeight() const noexcept { return m_height; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WINDOW_RESIZE);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);


private:
	i32 m_width, m_height;
};

class FrameBufferResizeEvent : public Event
{
public:
	FrameBufferResizeEvent(const i32 width, const i32 height)
		:
		m_width(width),
		m_height(height) {}

	const i32& GetWidth()  const noexcept { return m_width; }
	const i32& GetHeight() const noexcept { return m_height; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "FrameBufferResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(FRAME_BUFFER_RESIZE);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);


private:
	i32 m_width, m_height;
};

class WindowMoveEvent : public Event
{
public:
	WindowMoveEvent(const i32 x, const i32 y)
		:
		m_xPos(x),
		m_yPos(y) {}

	const i32& GetTopLeftX() const noexcept { return m_xPos; }
	const i32& GetTopLeftY() const noexcept { return m_yPos; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "WindowMoveEvent: (" << m_xPos << ", " << m_yPos << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(WINDOW_MOVED);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);

private:
	i32 m_xPos, m_yPos;
};

class WindowMaximizedEvent : public Event
{
public:
	explicit WindowMaximizedEvent(const bool maximized)
		:
		m_isMaximized(maximized) {}

	const bool& IsMaximized() const noexcept { return m_isMaximized; }

	String ToString() const override
	{
		if (m_isMaximized)
			return String("WindowMaximizeEvent: maximized");
		return String("WindowMaximizeEvent: unmaximized");
	}

	EVENT_CLASS_TYPE(WINDOW_MAXIMIZED);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);

private:
	bool m_isMaximized;
};

class WindowFocusLostEvent : public Event
{
public:
	WindowFocusLostEvent() {}
	EVENT_CLASS_TYPE(WINDOW_LOST_FOCUS);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);
};

class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent() {}
	EVENT_CLASS_TYPE(WINDOW_FOCUS);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WINDOW_CLOSE);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {}

	EVENT_CLASS_TYPE(APP_UPDATE);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}

	EVENT_CLASS_TYPE(APP_RENDER);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() {}

	EVENT_CLASS_TYPE(APP_TICK);
	EVENT_CLASS_CATEGORY(EC_APPLICATION);
};



NS_ENIGMA_END

#endif // !ENIGMA_APPLICATION_EVENT_H
