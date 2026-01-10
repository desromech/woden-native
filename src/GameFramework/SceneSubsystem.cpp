#include "Woden/GameFramework/SceneSubsystem.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace GameFramework
{
SceneSubsystem::SceneSubsystem()
{
    scene = SceneGraph::MakeScene();
}

SceneSubsystem::~SceneSubsystem()
{
}

} // End of namespace GameFramework
} // End of namespace Woden