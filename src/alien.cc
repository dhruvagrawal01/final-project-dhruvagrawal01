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
  // Set up as a static body because it is stationary
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);
  body_ = mWorld_->CreateBody(&bodyDef);

  // Used to identify the object type during collisions
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