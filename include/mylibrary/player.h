//
// Created by Dhruv Agrawal on 4/21/20.
//

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

#include <string>

namespace mylibrary {

class Player {
  b2Body* body;
  int x = 50;
  int y = 450;

 public:
  Player() : name("Random"), score(0), time(0) {}
  Player(const std::string& name, size_t score, double time)
      : name(name), score(score), time(time) {}

  std::string name;
  size_t score;
  double time;

  void SetBody(b2World* mWorld_);
  b2Body* GetBody();
  void Reset();
};

}  // namespace mylibrary

#endif  // FINALPROJECT_PLAYER_H
