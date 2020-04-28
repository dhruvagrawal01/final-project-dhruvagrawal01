//
// Created by Dhruv Agrawal on 4/28/20.
//

#ifndef FINALPROJECT_ALIEN_H
#define FINALPROJECT_ALIEN_H

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
namespace mylibrary {

class Alien {
  b2Body* body;
  size_t x;
  size_t y;

 public:
  Alien(b2World* mWorld_, size_t x, size_t y) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);

    body = mWorld_->CreateBody(&bodyDef);

    body->SetUserData((void*)"alien");

    b2PolygonShape alien;
    alien.SetAsBox(20, 20);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &alien;
    fixtureDef.density = 1.0f;

    body->CreateFixture(&fixtureDef);
  }

  b2Body* GetBody() { return body; }
};

}  // namespace mylibrary

#endif  // FINALPROJECT_ALIEN_H
