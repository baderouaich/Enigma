#pragma once
#ifndef ENIGMA_CREATE_PTR_H
#define ENIGMA_CREATE_PTR_H

#include <Core/Core.hpp>
#include <memory>

NS_ENIGMA_BEGIN
/*
*	Provides a class with CreateShared(), CreateUnique() for creating smart pointers instances.
*	Or raw creation with Create().
*/
template<class Child>
class CreatePtr
{
public:
	template<typename ... Args>
	static constexpr std::shared_ptr<Child> CreateShared(Args && ... args)
	{
		return std::make_shared<Child>(std::forward<Args>(args)...);
	}

	template<typename ... Args>
	static constexpr std::unique_ptr<Child> CreateUnique(Args && ... args)
	{
		return std::make_unique<Child>(std::forward<Args>(args)...);
	}

	template<typename ... Args>
	static constexpr Child* Create(Args && ... args)
	{
		return new Child(std::forward<Args>(args)...);
	}
};
NS_ENIGMA_END

#endif // !ENIGMA_CREATE_PTR_H
