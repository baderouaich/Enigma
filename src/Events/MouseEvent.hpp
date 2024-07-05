#pragma once
#ifndef ENIGMA_MOUSE_EVENT_H
#define ENIGMA_MOUSE_EVENT_H

#include <Core/Core.hpp>
#include <Core/Types.hpp>
#include <Input/Mouse/MouseCodes.hpp>
#include <sstream>

#include "Event.hpp"

NS_ENIGMA_BEGIN

/** Mouse Movement Event */
class MouseMotionEvent : public Event {
  public:
    MouseMotionEvent(const float x, const float y)
        : m_mouseX(x),
          m_mouseY(y) {}

    float GetX() const noexcept { return m_mouseX; }
    float GetY() const noexcept { return m_mouseY; }

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "MouseMotionEvent: " << m_mouseX << ", " << m_mouseY;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MOUSE_MOTION)
    EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)

  private:
    float m_mouseX, m_mouseY;
};

/** Mouse Wheel Scroll Event */
class MouseWheelEvent : public Event {
  public:
    MouseWheelEvent(const float xOffset, const float yOffset)
        : m_XOffset(xOffset),
          m_YOffset(yOffset) {}

    float GetXOffset() const noexcept { return m_XOffset; }
    float GetYOffset() const noexcept { return m_YOffset; }

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "MouseWheelEvent: " << m_XOffset << ", " << m_YOffset;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MOUSE_WHEEL)
    EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)

  private:
    float m_XOffset, m_YOffset;
};

/** Mouse Button Click, Hold Event */
class MouseButtonEvent : public Event {
  public:
    MouseCode GetMouseButton() const noexcept { return m_button; }

    virtual std::string ToString() const override = 0;

    EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::INPUT)
  protected:
    explicit MouseButtonEvent(const MouseCode button)
        : m_button(button) {}

    MouseCode m_button;
};

/** Mouse Button Press Event */
class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    explicit MouseButtonPressedEvent(const MouseCode button)
        : MouseButtonEvent(button) {}

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "MouseButtonPressedEvent: " << m_button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_PRESSED)
};

/** Mouse Button Release Event */
class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    explicit MouseButtonReleasedEvent(const MouseCode button)
        : MouseButtonEvent(button) {}

    std::string ToString() const override {
      std::ostringstream ss;
      ss << "MouseButtonReleasedEvent: " << m_button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_RELEASED)
};

NS_ENIGMA_END

#endif // !ENIGMA_MOUSE_EVENT_H
