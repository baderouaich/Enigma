#include <pch.hpp>
#include "Scene.hpp"

NS_ENIGMA_BEGIN


Scene::Scene() noexcept
	:
	m_quit(false),
	m_isLoading(false),
	m_mutex()
{
}

NS_ENIGMA_END


