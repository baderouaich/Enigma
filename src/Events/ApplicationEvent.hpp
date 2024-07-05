#pragma once
#ifndef ENIGMA_APPLICATION_EVENT_H
#define ENIGMA_APPLICATION_EVENT_H

#include <Core/Core.hpp>
#include <Core/Types.hpp>
#include <Utility/FileUtils.hpp> // fs::path
#include <sstream>

#include "Event.hpp"

NS_ENIGMA_BEGIN

/** Window size changed Event */
class WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(const std::int32_t width, const std::int32_t height)
        : m_width(width),
          m_height(height) {}

    std::int32_t GetWidth() const noexcept { return m_width; }
    std::int32_t GetHeight() const noexcept { return m_height; }

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "WindowResizeEvent: " << m_width << ", " << m_height;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_RESIZE);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


  private:
    std::int32_t m_width, m_height;
};

/** Frame buffer size changed Event */
class FrameBufferResizeEvent : public Event {
  public:
    FrameBufferResizeEvent(const std::int32_t width, const std::int32_t height)
        : m_width(width),
          m_height(height) {}

    std::int32_t GetWidth() const noexcept { return m_width; }
    std::int32_t GetHeight() const noexcept { return m_height; }

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "FrameBufferResizeEvent: " << m_width << ", " << m_height;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::FRAME_BUFFER_RESIZE);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


  private:
    std::int32_t m_width, m_height;
};

/** Window position changed Event */
class WindowMoveEvent : public Event {
  public:
    WindowMoveEvent(const std::int32_t x, const std::int32_t y)
        : m_xPos(x),
          m_yPos(y) {}

    std::int32_t GetTopLeftX() const noexcept { return m_xPos; }
    std::int32_t GetTopLeftY() const noexcept { return m_yPos; }

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "WindowMoveEvent: (" << m_xPos << ", " << m_yPos << ")";
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_MOVED);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

  private:
    std::int32_t m_xPos, m_yPos;
};

/** Window maximized Event */
class WindowMaximizedEvent : public Event {
  public:
    explicit WindowMaximizedEvent(const bool maximized)
        : m_isMaximized(maximized) {}

    bool IsMaximized() const noexcept { return m_isMaximized; }

    std::string ToString() const override {
      if (m_isMaximized)
        return std::string("WindowMaximizeEvent: maximized");
      return std::string("WindowMaximizeEvent: unmaximized");
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_MAXIMIZED);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

  private:
    bool m_isMaximized;
};

/** Window lost focus Event */
class WindowFocusLostEvent : public Event {
  public:
    WindowFocusLostEvent() {}
    EVENT_CLASS_TYPE(EventType::WINDOW_LOST_FOCUS);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Window gain focus Event */
class WindowFocusEvent : public Event {
  public:
    WindowFocusEvent() {}
    EVENT_CLASS_TYPE(EventType::WINDOW_FOCUS);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Window close button pressed Event */
class WindowCloseEvent : public Event {
  public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(EventType::WINDOW_CLOSE);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Files dropped over window Event */
class WindowFileDropEvent : public Event {
  public:
    explicit WindowFileDropEvent(const std::vector<fs::path>& filenames)
        : m_filenames(filenames) {}

    const std::vector<fs::path>& GetFilenames() const noexcept { return m_filenames; }

    EVENT_CLASS_TYPE(EventType::WINDOW_FILE_DROP);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

  private:
    std::vector<fs::path> m_filenames;
};

/** Application main loop tick Update Event */
class AppUpdateEvent : public Event {
  public:
    AppUpdateEvent() {}

    EVENT_CLASS_TYPE(EventType::APP_UPDATE);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Application main loop tick Draw Event */
class AppRenderEvent : public Event {
  public:
    AppRenderEvent() {}

    EVENT_CLASS_TYPE(EventType::APP_RENDER);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};

/** Application main loop tick Event */
class AppTickEvent : public Event {
  public:
    AppTickEvent() {}

    EVENT_CLASS_TYPE(EventType::APP_TICK);
    EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);
};


NS_ENIGMA_END

#endif // !ENIGMA_APPLICATION_EVENT_H
