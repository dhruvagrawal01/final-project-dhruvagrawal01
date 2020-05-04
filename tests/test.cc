// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <cinder/Rand.h>
#include <mylibrary/alien.h>
#include <mylibrary/engine.h>
#include <mylibrary/example.h>
#include <mylibrary/player.h>

#include <catch2/catch.hpp>

TEST_CASE("Player", "[Player]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  mylibrary::Player player;
  b2Vec2 position = {50, 450};

  SECTION("Testing SetBody") {
    player.SetBody(world);
    b2Body* body = player.GetBody();
    REQUIRE(body->GetType() == b2_kinematicBody);
    REQUIRE(body->GetUserData() == "player");
    REQUIRE(body->GetPosition() == position);
  }

  SECTION("Testing Reset") {
    player.time = 20;
    player.score = 500;
    player.Reset();
    REQUIRE(player.time == 0);
    REQUIRE(player.score == 0);
  }
}

TEST_CASE("Alien", "[Alien]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  b2Vec2 position = {100, 100};
  mylibrary::Alien alien = mylibrary::Alien(world, 100, 100);
  b2Body* body = alien.GetBody();
  REQUIRE(body->GetType() == b2_staticBody);
  REQUIRE(body->GetUserData() == "alien");
  REQUIRE(body->GetPosition() == position);
}

TEST_CASE("Shield", "[Shield]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  b2Vec2 position = {100, 100};
  mylibrary::Shield shield = mylibrary::Shield(world, 100, 100);
  b2Body* body = shield.GetBody();
  REQUIRE(body->GetType() == b2_staticBody);
  REQUIRE(body->GetUserData() == "shield");
  REQUIRE(body->GetPosition() == position);
}

TEST_CASE("Engine", "[Engine]") {
  mylibrary::Engine engine = mylibrary::Engine(800, 800);
  b2World* world = engine.mWorld_;

  SECTION("Testing AddAlien") {
    engine.AddAlien();

    std::vector<b2Body*> aliens = engine.GetAliens();
    std::vector<b2Body*> first_row = engine.GetFirstRow();

    b2Vec2 position1 = {730, 100};
    REQUIRE(aliens.at(0)->GetType() == b2_staticBody);
    REQUIRE(aliens.at(0)->GetUserData() == "alien");
    REQUIRE(aliens.at(0)->GetPosition() == position1);

    b2Vec2 position2 = {520, 100};
    REQUIRE(aliens.at(3)->GetPosition() == position2);
    REQUIRE(first_row.at(0)->GetPosition() == position2);
  }

  SECTION("Testing AddShield") {
    engine.AddShield();

    std::vector<b2Body*> shields = engine.GetShields();

    b2Vec2 position = {200, 200};
    REQUIRE(shields.at(0)->GetType() == b2_staticBody);
    REQUIRE(shields.at(0)->GetUserData() == "shield");
    REQUIRE(shields.at(0)->GetPosition() == position);
  }

  SECTION("Testing AddBullet") {
    engine.AddBullet(50, 50, false);
    engine.AddBullet(50, 100, true);

    std::vector<b2Body*> bullets = engine.GetBullets();

    b2Vec2 position1 = {50, 50};
    REQUIRE(bullets.at(0)->GetType() == b2_dynamicBody);
    REQUIRE(!bullets.at(0)->GetUserData());
    REQUIRE(bullets.at(0)->GetPosition() == position1);

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
    REQUIRE(engine.Step() == mylibrary::ResultantAction::RandomCollision);
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