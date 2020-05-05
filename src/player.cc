//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <spaceimpact/player.h>

namespace spaceimpact {

const int kPlayerSize = 40;

void Player::SetBody(b2World* mWorld_) {
  b2BodyDef bodyDef;
  // Set up as a kinematic body because it moves but doesn't react to forces
  bodyDef.type = b2_kinematicBody;
  bodyDef.position.Set(x_, y_);
  body_ = mWorld_->CreateBody(&bodyDef);

  // Used to identify the object type during collisions
  body_->SetUserData((void*)"player");

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(kPlayerSize, kPlayerSize);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  body_->CreateFixture(&fixtureDef);
}

b2Body* Player::GetBody() const { return body_; }

void Player::Reset() {
  score_ = 0;
  time_ = 0;
}

}  // namespace spaceimpact