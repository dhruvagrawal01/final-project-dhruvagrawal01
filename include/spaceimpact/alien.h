//
// Created by Dhruv Agrawal on 4/28/20.
//

#ifndef FINALPROJECT_ALIEN_H
#define FINALPROJECT_ALIEN_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

namespace spaceimpact {

class Alien {
  b2Body* body_;
  size_t x_;
  size_t y_;

 public:
  Alien(b2World* mWorld_, size_t x, size_t y);
  b2Body* GetBody();
};

}  // namespace spaceimpact

#endif  // FINALPROJECT_ALIEN_H