//
// Created by Dhruv Agrawal on 5/6/20.
//

#include <spaceimpact/alien.h>

#include <catch2/catch.hpp>

TEST_CASE("Alien", "[Alien]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  b2Vec2 position = {100, 100};
  spaceimpact::Alien alien = spaceimpact::Alien(world, 100, 100);
  // To test if the constructor creates the right b2Body or not
  b2Body* body = alien.GetBody();
  REQUIRE(body->GetType() == b2_staticBody);
  REQUIRE(body->GetUserData() == "alien");
  REQUIRE(body->GetPosition() == position);
}