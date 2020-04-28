//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <mylibrary/shield.h>

namespace mylibrary {

Shield::Shield(b2World* mWorld_, size_t x, size_t y) {
  this->x = x;
  this->y = y;
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);

  body = mWorld_->CreateBody(&bodyDef);
  body->SetUserData((void*)"shield");

  b2PolygonShape shield;
  shield.SetAsBox(20, 30);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shield;
  fixtureDef.density = 1.0f;

  body->CreateFixture(&fixtureDef);
}

b2Body* Shield::GetBody() { return body; }

}  // namespace mylibrary