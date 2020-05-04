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
  ResultantAction Step();
  void Reset();
  void AddBullet(int x, int y, bool is_alien);
  void AddAlien();
  void AddShield();
  const std::vector<b2Body *> &GetBullets() const;
  const std::vector<b2Body *> &GetAliens() const;
  const std::vector<b2Body *> &GetFirstRow() const;
  const std::vector<b2Body *> &GetShields() const;
  const void RemoveBullet(b2Body *bullet);
  b2World *mWorld_;

 private:
  const size_t width_;
  const size_t height_;
  std::vector<b2Body *> mBullets_;
  std::vector<b2Body *> mAliens_;
  std::vector<b2Body *> first_row_;
  std::vector<b2Body *> mShields_;

  ResultantAction TakeAction(b2Contact *contact);
};

}  // namespace mylibrary

#endif  // FINALPROJECT_ENGINE_H
