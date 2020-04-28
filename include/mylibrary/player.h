//
// Created by Dhruv Agrawal on 4/21/20.
//

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

#include <string>

namespace mylibrary {

class Player {
  b2Body* body;

 public:
  Player() : name("Random"), score(0), time(0) {}
  Player(const std::string& name, size_t score, double time)
      : name(name), score(score), time(time) {}
  std::string name;
  size_t score;
  double time;

  size_t x;
  size_t y;

  void SetBody(b2World* mWorld_) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    // bodyDef.position.Set(location.Row(), location.Col());
    bodyDef.position.Set(x, y);

    body = mWorld_->CreateBody(&bodyDef);

    body->SetUserData((void*)"player");

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(40, 40);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;

    body->CreateFixture(&fixtureDef);
  }
  b2Body* GetBody() { return body; }
};

}  // namespace mylibrary

#endif  // FINALPROJECT_PLAYER_H
