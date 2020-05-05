//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <spaceimpact/shield.h>

namespace spaceimpact {

const int kShieldSize = 25;

Shield::Shield(b2World* mWorld_, size_t x, size_t y) {
  this->x_ = x;
  this->y_ = y;
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);

  body_ = mWorld_->CreateBody(&bodyDef);
  body_->SetUserData((void*)"shield");

  b2PolygonShape shield;
  shield.SetAsBox(kShieldSize, kShieldSize);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shield;
  fixtureDef.density = 1.0f;

  body_->CreateFixture(&fixtureDef);
}

b2Body* Shield::GetBody() { return body_; }

}  // namespace spaceimpact