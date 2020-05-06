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

/**
 * Enum to represent the resulting action of a collision
 */
enum class ResultantAction {
  AlienKilled,
  PlayerKilled,
  ShieldLost,
  RandomCollision,
};

class Engine {
 public:
  /**
   * Constructor to create an Engine object
   * @param width The game window's width
   * @param height The game window's height
   */
  Engine(size_t width, size_t height);

  /**
   * Advances the engine's state by determining collisions
   * @return The action to take based on a collision
   */
  ResultantAction Step();

  /**
   * Adds bullets as pointers to b2Bodys
   * @param x The x-coordinate to add the bullet to
   * @param y The y-coordinate to add the bullet to
   * @param is_alien Whether the bullet is shot by an alien or the player
   */
  void AddBullet(int x, int y, bool is_alien);

  /**
   * Adds all aliens as pointers to b2Bodys
   */
  void AddAlien();

  /**
   * Adds all shields as pointers to b2Bodys
   */
  void AddShield();

  /**
   * Removes a bullet from the vector of bullets
   * @param bullet A pointer to the b2Body of the bullet to be removed
   */
  void RemoveBullet(b2Body *bullet);

  /**
   * Resets the state of the engine by reinitializing variables
   */
  void Reset();

  b2World *GetWorld() const;
  const std::vector<b2Body *> &GetBullets() const;
  const std::vector<b2Body *> &GetAliens() const;
  const std::vector<b2Body *> &GetFirstRow() const;
  const std::vector<b2Body *> &GetShields() const;

 private:
  const size_t width_;
  const size_t height_;
  b2World *world_;

 private:
  std::vector<b2Body *> bullets_;
  std::vector<b2Body *> aliens_;
  // The first row of aliens (which shoot bullets back)
  std::vector<b2Body *> first_row_;
  std::vector<b2Body *> shields_;

  /**
   * Takes the appropriate action based on the collision
   * @param contact Pointer to b2Contact between bodies
   * @return The resulting action to take
   */
  ResultantAction TakeAction(b2Contact *contact);
};

}  // namespace spaceimpact

#endif  // FINALPROJECT_ENGINE_H