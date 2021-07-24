#pragma once
#include <Core/Core.hpp>
#include <Scenes/Scene.hpp>
#include <imgui.h>

/*
*	Tool collapsing header view abstract class
*/
NS_ENIGMA_BEGIN
class ENIGMA_API Tool
{
public:
	Tool() = default;
	virtual ~Tool() = default;

public:
	virtual void OnCreate() = 0;
	virtual void OnDraw(Scene* parent) = 0;
	virtual void OnDestroy() = 0;
};
NS_ENIGMA_END
