//
// Created by Dhruv Agrawal on 5/3/20.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <mylibrary/alien.h>
#include <mylibrary/shield.h>

#include <vector>

namespace mylibrary {

enum class ResultantAction {
  AlienKilled,
  PlayerKilled,
  ShieldLost,
  RandomCollision
};

class Engine {
 public:
  Engine(size_t width, size_t height);
  b2World *mWorld_;

  ResultantAction Step();
  void Reset();
  void AddBullet(int x, int y, bool is_alien);
  //  void AddShip();
  void AddAlien();
  void AddShield();
  std::vector<b2Body *> mBullets_;
  std::vector<b2Body *> mAliens_;
  std::vector<b2Body *> first_row_;
  std::vector<b2Body *> mShields_;

 private:
  ResultantAction TakeAction(b2Contact *contact);

  const size_t width_;
  const size_t height_;
};

}  // namespace mylibrary

#endif  // FINALPROJECT_ENGINE_H
