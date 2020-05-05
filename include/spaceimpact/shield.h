//
// Created by Dhruv Agrawal on 4/28/20.
//

#ifndef FINALPROJECT_SHIELD_H
#define FINALPROJECT_SHIELD_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

namespace spaceimpact {

class Shield {
  // A pointer to the b2Body object of the shield
  b2Body* body_;
  // The player's x-coordinate
  size_t x_;
  // The player's y-coordinate
  size_t y_;

 public:
  Shield(b2World* mWorld_, size_t x, size_t y);
  b2Body* GetBody();
};

}  // namespace spaceimpact

#endif  // FINALPROJECT_SHIELD_H