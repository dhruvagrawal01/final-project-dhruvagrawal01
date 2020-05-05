//
// Created by Dhruv Agrawal on 5/3/20.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <spaceimpact/alien.h>
#include <spaceimpact/shield.h>

#include <vector>

namespace spaceimpact {

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
  void AddBullet(int x, int y, bool is_alien);
  void AddAlien();
  void AddShield();
  void RemoveBullet(b2Body *bullet);
  void Reset();
  b2World *GetWorld() const;
  const std::vector<b2Body *> &GetBullets() const;
  const std::vector<b2Body *> &GetAliens() const;
  const std::vector<b2Body *> &GetFirstRow() const;
  const std::vector<b2Body *> &GetShields() const;

 private:
  const size_t width_;
  const size_t height_;
  b2World *mWorld_;

 private:
  std::vector<b2Body *> mBullets_;
  std::vector<b2Body *> mAliens_;
  std::vector<b2Body *> first_row_;
  std::vector<b2Body *> mShields_;

  ResultantAction TakeAction(b2Contact *contact);
};

}  // namespace spaceimpact

#endif  // FINALPROJECT_ENGINE_H