// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <spaceimpact/engine.h>

#include <catch2/catch.hpp>

TEST_CASE("Engine", "[Engine]") {
  spaceimpact::Engine engine = spaceimpact::Engine(800, 800);
  b2World* world = engine.GetWorld();

  SECTION("Testing AddAlien") {
    engine.AddAlien();

    std::vector<b2Body*> aliens = engine.GetAliens();
    std::vector<b2Body*> first_row = engine.GetFirstRow();

    // To test if the aliens_ vector is populated correctly
    b2Vec2 position1 = {730, 100};
    REQUIRE(aliens.at(0)->GetType() == b2_staticBody);
    REQUIRE(aliens.at(0)->GetUserData() == "alien");
    REQUIRE(aliens.at(0)->GetPosition() == position1);

    // To test if the first_row_ vector is populated correctly
    b2Vec2 position2 = {520, 100};
    REQUIRE(aliens.at(3)->GetPosition() == position2);
    REQUIRE(first_row.at(0)->GetPosition() == position2);
  }

  SECTION("Testing AddShield") {
    engine.AddShield();

    std::vector<b2Body*> shields = engine.GetShields();

    // To test if the shields_ vector is populated correctly
    b2Vec2 position = {200, 200};
    REQUIRE(shields.at(0)->GetType() == b2_staticBody);
    REQUIRE(shields.at(0)->GetUserData() == "shield");
    REQUIRE(shields.at(0)->GetPosition() == position);
  }

  SECTION("Testing AddBullet") {
    engine.AddBullet(50, 50, false);
    engine.AddBullet(50, 100, true);

    std::vector<b2Body*> bullets = engine.GetBullets();

    // To test if the bullets_ vector is populated correctly
    // Tests for an alien's bullet
    b2Vec2 position1 = {50, 50};
    REQUIRE(bullets.at(0)->GetType() == b2_dynamicBody);
    REQUIRE(!bullets.at(0)->GetUserData());
    REQUIRE(bullets.at(0)->GetPosition() == position1);

    // Tests for the player's bullet
    b2Vec2 position2 = {50, 100};
    REQUIRE(bullets.at(1)->GetType() == b2_dynamicBody);
    REQUIRE(bullets.at(1)->GetUserData());
    REQUIRE(bullets.at(1)->GetPosition() == position2);
  }

  SECTION("Testing RemoveBullet") {
    engine.AddBullet(50, 50, false);
    engine.RemoveBullet(engine.GetBullets().at(0));

    REQUIRE(engine.GetBullets().empty());
  }

  SECTION("Testing Step") {
    REQUIRE(engine.Step() == spaceimpact::ResultantAction::RandomCollision);
  }

  SECTION("Testing Reset") {
    engine.AddAlien();
    engine.AddShield();
    engine.AddBullet(50, 50, false);

    engine.Reset();
    REQUIRE(engine.GetAliens().empty());
    REQUIRE(engine.GetFirstRow().empty());
    REQUIRE(engine.GetShields().empty());
    REQUIRE(engine.GetBullets().empty());
  }
}