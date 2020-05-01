//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <mylibrary/alien.h>

namespace mylibrary {

Alien::Alien(b2World* mWorld_, size_t x, size_t y) {
  this->x = x;
  this->y = y;

  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
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

b2Body* Alien::GetBody() { return body; }

}  // namespace mylibrary