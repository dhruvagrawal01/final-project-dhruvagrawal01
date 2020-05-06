//
// Created by Dhruv Agrawal on 4/21/20.
//

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Cinder/app/App.h>

#include <string>

namespace spaceimpact {

class Player {
 private:
  // A pointer to the b2Body object of the player
  b2Body* body_;
  // The player's x-coordinate
  int x_ = 50;
  // The player's y-coordinate
  // int y_ = (cinder::app::getWindowHeight() / 2) + 50;
  int y_ = 450;

 public:
  Player() : name_("Random"), score_(0), time_(0) {}

  Player(const std::string& name, size_t score, double time)
      : name_(name), score_(score), time_(time) {}

  std::string name_;
  size_t score_;
  double time_;

  void SetBody(b2World* world_);
  b2Body* GetBody() const;

  /**
   * Resets the player object
   */
  void Reset();
};

}  // namespace spaceimpact

#endif  // FINALPROJECT_PLAYER_H