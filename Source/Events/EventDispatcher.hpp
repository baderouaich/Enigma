#pragma once
#ifndef ENIGMA_EVENT_DISPATCHER_H
#define ENIGMA_EVENT_DISPATCHER_H

#include <Core/Core.hpp>
#include <Core/Types.hpp>

#include <functional>

#include "Event.hpp"

NS_ENIGMA_BEGIN

/** Event dispatcher to dispatch specific desired events */
class ENIGMA_API EventDispatcher
{
	template<typename T>
	using EventFun = std::function<bool(T&)>;

public:
	explicit EventDispatcher(Event& event)
		:
		m_event(event)
	{
	}

	template<typename T>
	bool Dispatch(EventFun<T> func)
	{
		if (m_event.GetEventType() == T::GetStaticType())
		{
			m_event.SetHandled(func(*(T*)&m_event));
			return true;
		}
		return false;
	}
private:
	Event& m_event;
};
NS_ENIGMA_END

#endif // !ENIGMA_EVENT_DISPATCHER_H
