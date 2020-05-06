//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <spaceimpact/shield.h>

namespace spaceimpact {

const int kShieldSize = 25;

Shield::Shield(b2World* world_, size_t x, size_t y) {
  this->x_ = x;
  this->y_ = y;

  b2BodyDef bodyDef;
  // Set up as a static body because it is stationary
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);
  body_ = world_->CreateBody(&bodyDef);

  // Used to identify the object type during collisions
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