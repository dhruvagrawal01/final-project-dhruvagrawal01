//
// Created by Dhruv Agrawal on 4/28/20.
//

#ifndef FINALPROJECT_ALIEN_H
#define FINALPROJECT_ALIEN_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

namespace mylibrary {

class Alien {
  b2Body* body;
  size_t x;
  size_t y;

 public:
  Alien(b2World* mWorld_, size_t x, size_t y);
  b2Body* GetBody();
};

}  // namespace mylibrary

#endif  // FINALPROJECT_ALIEN_H
