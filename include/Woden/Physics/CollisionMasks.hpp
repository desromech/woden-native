#ifndef WODEN_PHYSICS_COLLISION_MASKS_HPP
#define  WODEN_PHYSICS_COLLISION_MASKS_HPP

#include <stdint.h>

namespace Woden
{
namespace Physics
{
/**
 * Collision Masks
 */
enum class CollisionMasks : uint32_t
{
	Static = 1<<0,
	Kinematic = 1<<1,
	RigidBody = 1<<2,
	CharacterObject = 1<<3,
	Vehicle = 1<<4,
	Sensor = 1<<5,
	
	AllSolid = Static | Kinematic | RigidBody | Vehicle | CharacterObject,
	AllNonSolid = Sensor,

	DefaultCollisionGroup = RigidBody,
	DefaultCollisionResponseGroup = AllSolid,
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_COLLISION_MASKS_HPP