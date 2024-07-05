#include "Scene.hpp"
#include <pch.hpp>

NS_ENIGMA_BEGIN

Scene::Scene() noexcept
    : m_mutex{},
      m_quit{false},
      m_isLoading{false} {
}

NS_ENIGMA_END
