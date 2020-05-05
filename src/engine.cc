//
// Created by Dhruv Agrawal on 5/3/20.
//

#include <spaceimpact/engine.h>

namespace spaceimpact {

const float kRadius = 3.0f;
const int kAlienGap = 70;
const int kShieldGap = 200;
const int kNumAliensPerRow = 10;
const int kNumAlienRows = 4;
const int kNumShields = 4;

Engine::Engine(size_t width, size_t height) : width_{width}, height_{height} {
  b2Vec2 gravity(0.0f, 0.0f);
  mWorld_ = new b2World(gravity);
}

ResultantAction Engine::Step() {
  // Taken from:
  // https://www.iforce2d.net/b2dtut/
  for (b2Contact* contact = mWorld_->GetContactList(); contact;
       contact = contact->GetNext()) {
    if (contact->IsTouching()) {
      return TakeAction(contact);
    }
  }
  return ResultantAction::RandomCollision;
}

ResultantAction Engine::TakeAction(b2Contact* contact) {
  b2Body* b1 = contact->GetFixtureA()->GetBody();
  b2Body* b2 = contact->GetFixtureB()->GetBody();

  if (b1->GetUserData() == "alien" && !b2->GetUserData()) {
    // Collision between an alien and the player's bullet
    mAliens_.erase(std::remove(mAliens_.begin(), mAliens_.end(), b1),
                   mAliens_.end());
    first_row_.erase(std::remove(first_row_.begin(), first_row_.end(), b1),
                     first_row_.end());
    RemoveBullet(b2);
    mWorld_->DestroyBody(b1);
    mWorld_->DestroyBody(b2);
    return ResultantAction::AlienKilled;
  } else if (b1->GetUserData() == "player" && b2->GetUserData()) {
    // Collision between the player and an alien's bullet
    return ResultantAction::PlayerKilled;
  } else if (b1->GetUserData() == "shield") {
    // Collision between a shield and any bullet
    mShields_.erase(std::remove(mShields_.begin(), mShields_.end(), b1),
                    mShields_.end());
    RemoveBullet(b2);
    mWorld_->DestroyBody(b1);
    mWorld_->DestroyBody(b2);
    return ResultantAction::ShieldLost;
  } else if ((b1->GetUserData() && !b2->GetUserData()) ||
             (!b1->GetUserData() && b2->GetUserData())) {
    // Collision between an alien's bullet and the player's bullet
    RemoveBullet(b1);
    RemoveBullet(b2);
    mWorld_->DestroyBody(b1);
    mWorld_->DestroyBody(b2);
  }
  return ResultantAction::RandomCollision;
}

void Engine::AddBullet(int x, int y, bool is_alien) {
  // Taken from:
  // https://github.com/cinder/Cinder/tree/master/blocks/Box2D/

  b2BodyDef bodyDef;
  // Set up as a dynamic body as it moves and may react to forces
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);
  b2Body* body = mWorld_->CreateBody(&bodyDef);

  // Used to identify the object type during collisions
  // Sets it as 'true' if an alien's bullet
  // Sets it as 'false' if the player's bullet
  body->SetUserData((void*)is_alien);

  b2CircleShape bullet;
  bullet.m_p.Set(kRadius, kRadius);
  bullet.m_radius = kRadius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &bullet;
  fixtureDef.density = 1.0f;
  body->CreateFixture(&fixtureDef);

  mBullets_.push_back(body);
}

void Engine::AddAlien() {
  for (size_t x = 0; x < kNumAliensPerRow; x++) {
    for (size_t y = 0; y < kNumAlienRows; y++) {
      spaceimpact::Alien alien = spaceimpact::Alien(
          mWorld_, width_ - kAlienGap * (y + 1), x * kAlienGap + 100);
      // Checks whether the alien is in the first row
      if (y == kNumAlienRows - 1) {
        first_row_.push_back(alien.GetBody());
      }
      mAliens_.push_back(alien.GetBody());
    }
  }
}

void Engine::AddShield() {
  for (int y = 0; y < kNumShields; y++) {
    spaceimpact::Shield shield =
        spaceimpact::Shield(mWorld_, kShieldGap, y * 150 + kShieldGap);
    mShields_.push_back(shield.GetBody());
  }
}

void Engine::RemoveBullet(b2Body* bullet) {
  mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), bullet),
                  mBullets_.end());
}

void Engine::Reset() {
  mAliens_.clear();
  mBullets_.clear();
  mShields_.clear();
  first_row_.clear();
  b2Vec2 gravity(0.0f, 0.0f);
  mWorld_ = new b2World(gravity);
}

b2World* Engine::GetWorld() const { return mWorld_; }

const std::vector<b2Body*>& Engine::GetBullets() const { return mBullets_; }

const std::vector<b2Body*>& Engine::GetAliens() const { return mAliens_; }

const std::vector<b2Body*>& Engine::GetFirstRow() const { return first_row_; }

const std::vector<b2Body*>& Engine::GetShields() const { return mShields_; }

}  // namespace spaceimpact