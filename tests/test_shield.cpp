//
// Created by Dhruv Agrawal on 5/6/20.
//

#include <spaceimpact/shield.h>

#include <catch2/catch.hpp>

TEST_CASE("Shield", "[Shield]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  b2Vec2 position = {100, 100};
  spaceimpact::Shield shield = spaceimpact::Shield(world, 100, 100);
  // To test if the constructor creates the right b2Body or not
  b2Body* body = shield.GetBody();
  REQUIRE(body->GetType() == b2_staticBody);
  REQUIRE(body->GetUserData() == "shield");
  REQUIRE(body->GetPosition() == position);
}