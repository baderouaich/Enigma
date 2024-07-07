#pragma once
#include <Core/Core.hpp>
#include <Scenes/Scene.hpp>
#include <imgui.h>

/**
*	Tool collapsing header view abstract class
*/
NS_ENIGMA_BEGIN
class Tool {
  public:
    Tool() = default;
    virtual ~Tool() = default;

  public: // Tool life cycle
    /** Called when a tool is constructed */
    virtual void OnCreate() = 0;
    /** Called each frame to handle imgui draw
    *	@param parent: pointer to ToolsScene
    */
    virtual void OnDraw(Scene *parent) = 0;
    /** Called when tool is destroyed */
    virtual void OnDestroy() = 0;
};
NS_ENIGMA_END
