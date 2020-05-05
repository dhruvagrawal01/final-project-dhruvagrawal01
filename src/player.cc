//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <spaceimpact/player.h>

namespace spaceimpact {

void Player::SetBody(b2World* mWorld_) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_kinematicBody;
  //  bodyDef.type = b2_dynamicBody;

  // bodyDef.position.Set(location.Row(), location.Col());
  bodyDef.position.Set(x_, y_);

  body_ = mWorld_->CreateBody(&bodyDef);

  //  body->SetUserData((void*)"player");
  body_->SetUserData((void*)"player");

  b2PolygonShape dynamicBox;
  // 30,30? or change?
  dynamicBox.SetAsBox(30, 30);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;

  body_->CreateFixture(&fixtureDef);
}

b2Body* Player::GetBody() { return body_; }

void Player::Reset() {
  score_ = 0;
  time_ = 0;
}

}  // namespace spaceimpact