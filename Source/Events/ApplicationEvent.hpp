#pragma once
#ifndef ENIGMA_APPLICATION_EVENT_H
#define ENIGMA_APPLICATION_EVENT_H

#include <Core/Core.hpp>
#include <Core/Types.hpp>
#include <sstream>
#include <Utility/FileUtils.hpp> // fs::path

#include "Event.hpp"

NS_ENIGMA_BEGIN

/** Window size changed Event */
class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(const i32 width, const i32 height)
		:
		m_width(width),
		m_height(height) {}

	i32 GetWidth()  const noexcept { return m_width; }
	i32 GetHeight() const noexcept { return m_height; }

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "WindowResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::WINDOW_RESIZE);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


private:
	i32 m_width, m_height;
};

/** Frame buffer size changed Event */
class FrameBufferResizeEvent : public Event
{
public:
	FrameBufferResizeEvent(const i32 width, const i32 height)
		:
		m_width(width),
		m_height(height) {}

	i32 GetWidth()  const noexcept { return m_width; }
	i32 GetHeight() const noexcept { return m_height; }

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "FrameBufferResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::FRAME_BUFFER_RESIZE);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


private:
	i32 m_width, m_height;
};

/** Window position changed Event */
class WindowMoveEvent : public Event
{
public:
	WindowMoveEvent(const i32 x, const i32 y)
		:
		m_xPos(x),
		m_yPos(y) {}

	i32 GetTopLeftX() const noexcept { return m_xPos; }
	i32 GetTopLeftY() const noexcept { return m_yPos; }

	String ToString() const override
	{
		std::ostringstream ss;
		ss << "WindowMoveEvent: (" << m_xPos << ", " << m_yPos << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::WINDOW_MOVED);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

private:
	i32 m_xPos, m_yPos;
};

/** Window maximized Event */
class WindowMaximizedEvent : public Event
{
public:
	explicit WindowMaximizedEvent(const bool maximized)
		:
		m_isMaximized(maximized) {}

	bool IsMaximized() const noexcept { return m_isMaximized; }

	String ToString() const override
	{
		if (m_isMaximized)
			return String("WindowMaximizeEvent: maximized");
		return String("WindowMaximizeEvent: unmaximized");
	}

	EVENT_CLASS_TYPE(EventType::WINDOW_MAXIMIZED);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

private:
	bool m_isMaximized;
};

/** Window lost focus Event */
class WindowFocusLostEvent : public Event
{
public:
	WindowFocusLostEvent() {}
	EVENT_CLASS_TYPE(EventType::WINDOW_LOST_FOCUS);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Window gain focus Event */
class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent() {}
	EVENT_CLASS_TYPE(EventType::WINDOW_FOCUS);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Window close button pressed Event */
class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(EventType::WINDOW_CLOSE);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Files dropped over window Event */
class WindowFileDropEvent : public Event
{
public:
	explicit WindowFileDropEvent(const std::vector<fs::path>& filenames) 
		:
		m_filenames(filenames)
	{}

	const std::vector<fs::path>& GetFilenames() const noexcept { return m_filenames; }

	EVENT_CLASS_TYPE(EventType::WINDOW_FILE_DROP);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

private:
	std::vector<fs::path> m_filenames;
};

/** Application main loop tick Update Event */
class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {}

	EVENT_CLASS_TYPE(EventType::APP_UPDATE);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Application main loop tick Draw Event */
class AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}

	EVENT_CLASS_TYPE(EventType::APP_RENDER);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Application main loop tick Event */
class AppTickEvent : public Event
{
public:
	AppTickEvent() {}

	EVENT_CLASS_TYPE(EventType::APP_TICK);
	EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};


NS_ENIGMA_END

#endif // !ENIGMA_APPLICATION_EVENT_H
