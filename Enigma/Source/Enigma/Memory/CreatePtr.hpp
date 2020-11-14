#pragma once
#ifndef ENIGMA_CREATE_PTR_H
#define ENIGMA_CREATE_PTR_H

#include <Enigma/Core/Core.hpp>
#include <Enigma/Debug/Logger/Logger.hpp>

NS_ENIGMA_BEGIN
/*
*	Provides a class with a CreateShared() & CreateUnique()
*	for smart pointers creation.
*	Or raw creation with Create() only
*/
template<class Child>
class CreatePtr
{
public:
	template<typename ... Args>
	static constexpr SharedPtr<Child> CreateShared(Args && ... args)
	{
		return std::make_shared<Child>(std::forward<Args>(args)...);
	}

	template<typename ... Args>
	static constexpr UniquePtr<Child> CreateUnique(Args && ... args)
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
