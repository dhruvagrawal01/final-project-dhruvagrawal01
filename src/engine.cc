//
// Created by Dhruv Agrawal on 5/3/20.
//

#include <mylibrary/engine.h>

namespace mylibrary {

const float kRadius = 3;

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
    }  // do something with the contact
  }
  return ResultantAction::RandomCollision;
}

ResultantAction Engine::TakeAction(b2Contact* contact) {
  b2Body* b1 = contact->GetFixtureA()->GetBody();
  b2Body* b2 = contact->GetFixtureB()->GetBody();

  if (b1->GetUserData() == "alien" && !b2->GetUserData()) {
    mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                    mBullets_.end());
    mAliens_.erase(std::remove(mAliens_.begin(), mAliens_.end(), b1),
                   mAliens_.end());
    first_row_.erase(std::remove(first_row_.begin(), first_row_.end(), b1),
                     first_row_.end());
    mWorld_->DestroyBody(b1);
    mWorld_->DestroyBody(b2);
    return ResultantAction::AlienKilled;
  } else if (b1->GetUserData() == "player" && b2->GetUserData()) {
    return ResultantAction::PlayerKilled;
  } else if (b1->GetUserData() == "shield") {
    mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                    mBullets_.end());
    mShields_.erase(std::remove(mShields_.begin(), mShields_.end(), b1),
                    mShields_.end());
    mWorld_->DestroyBody(b1);
    mWorld_->DestroyBody(b2);
    return ResultantAction::ShieldLost;
  } else if (b1->GetUserData() && !b2->GetUserData()) {
    mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b1),
                    mBullets_.end());
    mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                    mBullets_.end());
  } else if (!b1->GetUserData() && b2->GetUserData()) {
    mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b1),
                    mBullets_.end());
    mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                    mBullets_.end());
  }
  // Check if both bullets collide
  return ResultantAction::RandomCollision;
}

void Engine::Reset() {
  mAliens_.clear();
  mBullets_.clear();
  mShields_.clear();
  first_row_.clear();
  b2Vec2 gravity(0.0f, 0.0f);
  mWorld_ = new b2World(gravity);
}

void Engine::AddBullet(int x, int y, bool is_alien) {
  // Taken from:
  // https://github.com/cinder/Cinder/tree/master/blocks/Box2D/

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);

  b2Body* body = mWorld_->CreateBody(&bodyDef);

  //  if (is_alien) {
  //    body->SetUserData((void*)"alien_shot");
  //  } else {
  //    body->SetUserData((void*)"player_shot");
  //  }
  body->SetUserData((void*)is_alien);

  b2CircleShape bullet;
  bullet.m_p.Set(3.0f, 3.0f);
  bullet.m_radius = kRadius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &bullet;
  fixtureDef.density = 1.0f;

  body->CreateFixture(&fixtureDef);
  mBullets_.push_back(body);
}

// void Engine::AddShip() {
//  player_.SetBody(mWorld_);
//
//  cinder::gl::color(0.68, 0.68, 0.68);
//  cinder::gl::pushModelMatrix();
//  cinder::gl::translate(player_.GetX(), player_.GetY());
//  cinder::Rectf drawRect(-40, -40, 40, 40);
//
//  cinder::gl::draw(ship_texture_, drawRect);
//  cinder::gl::popModelMatrix();
//}

void Engine::AddAlien() {
  for (size_t x = 0; x < 10; x++) {
    for (size_t y = 1; y < 5; y++) {
      // Change the loop conditions?
      mylibrary::Alien alien =
          mylibrary::Alien(mWorld_, width_ - 70 * y, x * 70 + 100);
      if (y == 4) {
        first_row_.push_back(alien.GetBody());
      }
      mAliens_.push_back(alien.GetBody());
    }
  }
}

void Engine::AddShield() {
  for (int y = 0; y < 4; y++) {
    mylibrary::Shield shield = mylibrary::Shield(mWorld_, 200, y * 150 + 200);
    mShields_.push_back(shield.GetBody());
  }
}

}  // namespace mylibrary