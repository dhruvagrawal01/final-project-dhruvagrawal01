//
// Created by Dhruv Agrawal on 4/28/20.
//

#include <mylibrary/player.h>

namespace mylibrary {

void Player::SetBody(b2World* mWorld_) {
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

b2Body* Player::GetBody() { return body; }

int Player::GetX() { return x; }

void Player::ChangeY(int toAdd) { y += toAdd; }

int Player::GetY() { return y; }

}  // namespace mylibrary