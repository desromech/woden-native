#ifndef WODEN_GAME_FRAMEWORK_SCENE_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_SCENE_SUBSYSTEM_HPP

#include "Subsystem.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace GameFramework
{

/**
 * I am a subsystem that deals with scene presentations
 */
class SceneSubsystem : public Subsystem 
{
public:
    SceneSubsystem();
    ~SceneSubsystem();
    
    SceneGraph::ScenePtr scene;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_SCENE_SUBSYSTEM_HPP
