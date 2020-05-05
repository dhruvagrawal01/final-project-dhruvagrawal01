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

namespace spaceimpact {

class Player {
  b2Body* body_;
  int x_ = 50;
  int y_ = 450;

 public:
  Player() : name_("Random"), score_(0), time_(0) {}
  Player(const std::string& name, size_t score, double time)
      : name_(name), score_(score), time_(time) {}

  std::string name_;
  size_t score_;
  double time_;

  void SetBody(b2World* mWorld_);
  b2Body* GetBody();
  void Reset();
};

}  // namespace spaceimpact

#endif  // FINALPROJECT_PLAYER_H