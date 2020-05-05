//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <spaceimpact/alien.h>

namespace spaceimpact {

const int kAlienSize = 20;

Alien::Alien(b2World* mWorld_, size_t x, size_t y) {
  this->x_ = x;
  this->y_ = y;

  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);

  body_ = mWorld_->CreateBody(&bodyDef);

  body_->SetUserData((void*)"alien");

  b2PolygonShape alien;
  alien.SetAsBox(kAlienSize, kAlienSize);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &alien;
  fixtureDef.density = 1.0f;

  body_->CreateFixture(&fixtureDef);
}

b2Body* Alien::GetBody() { return body_; }

}  // namespace spaceimpact