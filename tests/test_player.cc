//
// Created by Dhruv Agrawal on 5/6/20.
//

#include <spaceimpact/player.h>

#include <catch2/catch.hpp>

TEST_CASE("Player", "[Player]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  spaceimpact::Player player;
  b2Vec2 position = {50, 450};

  SECTION("Testing SetBody") {
    // To test if SetBody creates the right b2Body or not
    player.SetBody(world);
    b2Body* body = player.GetBody();
    REQUIRE(body->GetType() == b2_kinematicBody);
    REQUIRE(body->GetUserData() == "player");
    REQUIRE(body->GetPosition() == position);
  }

  SECTION("Testing Reset") {
    player.time_ = 20;
    player.score_ = 500;
    player.Reset();
    REQUIRE(player.time_ == 0);
    REQUIRE(player.score_ == 0);
  }
}