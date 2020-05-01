// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <cinder/Rand.h>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>

namespace myapp {

using cinder::app::KeyEvent;

const char kDbPath[] = "finalproject.db";
const char kNormalFont[] = "Arial";
const float kRadius = 3;
const int kGameOver = 0;
const int kPlaying = 1;
const char kAlienDead[] = "invaderkilled.wav";
const char kPlayerDead[] = "explosion.wav";

MyApp::MyApp()
    : leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      ship_texture_{
          cinder::gl::Texture2d::create(loadImage(loadAsset("ship.png")))},
      alien_texture_{
          cinder::gl::Texture2d::create(loadImage(loadAsset("alien.png")))},
      shield_texture_{
          cinder::gl::Texture2d::create(loadImage(loadAsset("shield.png")))},
      alien_killed{cinder::audio::load(cinder::app::loadAsset(kAlienDead))},
      player_killed{cinder::audio::load(cinder::app::loadAsset(kPlayerDead))},
      alien_killed_voice_{cinder::audio::Voice::create(alien_killed)},
      player_killed_voice_{cinder::audio::Voice::create(player_killed)} {}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();
  b2Vec2 gravity(0.0f, 0.0f);
  mWorld_ = new b2World(gravity);
  gameState = kPlaying;
  timer_.start();
  AddShield();
}

void MyApp::AddBullet(int x, int y, bool is_alien) {
  // Taken from:
  // https://github.com/cinder/Cinder/tree/master/blocks/Box2D/

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);

  b2Body* body = mWorld_->CreateBody(&bodyDef);

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

void MyApp::AddShip() {
  player_.SetBody(mWorld_);

  cinder::gl::color(0.68, 0.68, 0.68);
  cinder::gl::pushModelMatrix();
  cinder::gl::translate(player_.GetBody()->GetPosition().x,
                        player_.GetBody()->GetPosition().y);
  cinder::Rectf drawRect(-40, -40, 40, 40);
  // cinder::gl::draw(ship_texture_, drawRect);
  cinder::gl::draw(ship_texture_, drawRect);
  //  player_.GetBody()->SetLinearVelocity(b2Vec2(0.0f, 30.0f));
  cinder::gl::popModelMatrix();
}

void MyApp::AddAlien() {
  for (int i = 0; i < 10; i++) {
    for (int j = 1; j < 5; j++) {
      mylibrary::Alien alien =
          mylibrary::Alien(mWorld_, getWindowWidth() - 70 * j, i * 70 + 100);
      if (j == 4) {
        first_row_.push_back(alien.GetBody());
      }
      mAliens_.push_back(alien.GetBody());
    }
  }
}
void MyApp::AddShield() {
  for (int k = 0; k < 4; k++) {
    mylibrary::Shield shield = mylibrary::Shield(mWorld_, 200, k * 150 + 200);
    mShields_.push_back(shield.GetBody());
  }
}
void MyApp::update() {
  if (gameState == kGameOver) {
    return;
  }
  const auto time = std::chrono::system_clock::now();
  long double time_since_change =
      std::chrono::duration_cast<std::chrono::milliseconds>(time - last_time_)
          .count() /
      1000.0;

  if (timer_.getSeconds() > 2.5 && time_since_change >= 2.0 &&
      !first_row_.empty()) {
    int randBody = cinder::Rand::randInt(0, first_row_.size());
    b2Body* alien = first_row_.at(randBody);
    AddBullet(alien->GetPosition().x - 25, alien->GetPosition().y, true);
    last_time_ = time;
  }
  for (int i = 0; i < 10; ++i) {
    mWorld_->Step(1 / 30.0f, 10, 10);

    // Taken from:
    // https://www.iforce2d.net/b2dtut/
    for (b2Contact* contact = mWorld_->GetContactList(); contact;
         contact = contact->GetNext()) {
      if (contact->IsTouching()) {
        b2Body* b1 = contact->GetFixtureA()->GetBody();
        b2Body* b2 = contact->GetFixtureB()->GetBody();

        if (b1->GetUserData() == "alien" && !b2->GetUserData()) {
          mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                          mBullets_.end());
          mAliens_.erase(std::remove(mAliens_.begin(), mAliens_.end(), b1),
                         mAliens_.end());
          first_row_.erase(
              std::remove(first_row_.begin(), first_row_.end(), b1),
              first_row_.end());
          mWorld_->DestroyBody(b1);
          mWorld_->DestroyBody(b2);
          alien_killed_voice_->start();
          score_ += 25;
        } else if (b1->GetUserData() == "player" && b2->GetUserData()) {
          gameState = kGameOver;
          player_killed_voice_->start();
        } else if (b1->GetUserData() == "shield" && b2->GetUserData()) {
          mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                          mBullets_.end());
          mShields_.erase(std::remove(mShields_.begin(), mShields_.end(), b1),
                          mShields_.end());
          mWorld_->DestroyBody(b1);
          mWorld_->DestroyBody(b2);
        } else if (b1->GetUserData() == "shield" && !b2->GetUserData()) {
          mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), b2),
                          mBullets_.end());
          mWorld_->DestroyBody(b2);
        }
        // Check if both bullets collide
      }  // do something with the contact
    }
  }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  if (gameState == kGameOver) {
    cinder::gl::clear(cinder::Color(1, 0, 0));
    DrawGameOver();
    return;
  }
  cinder::gl::clear();

  DrawMyScore();
  DrawTime();

  AddShip();

  if (mAliens_.empty()) {
    AddAlien();
  }

  for (const auto& bullet : mBullets_) {
    cinder::gl::color(1, 0, 0);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(bullet->GetPosition().x, bullet->GetPosition().y);
    cinder::gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);
    if (bullet->GetUserData()) {
      bullet->SetLinearVelocity(b2Vec2(-40.0f, 0.0f));
    } else {
      bullet->SetLinearVelocity(b2Vec2(40.0f, 0.0f));
    }
    if (bullet->GetPosition().x > getWindowWidth() ||
        bullet->GetPosition().x < 0) {
      mBullets_.erase(std::remove(mBullets_.begin(), mBullets_.end(), bullet),
                      mBullets_.end());
    }
    cinder::gl::popModelMatrix();
  }

  for (const auto& alien : mAliens_) {
    cinder::gl::color(1, 1, 1);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(alien->GetPosition().x, alien->GetPosition().y);
    cinder::Rectf drawRect(-20, -20, 20, 20);
    cinder::gl::draw(alien_texture_, drawRect);
    cinder::gl::popModelMatrix();
  }

  for (const auto& shield : mShields_) {
    cinder::gl::color(0, 1, 0);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(shield->GetPosition().x, shield->GetPosition().y);
    cinder::Rectf drawRect(-20, -30, 20, 30);
    cinder::gl::draw(shield_texture_, drawRect);
    cinder::gl::popModelMatrix();
  }
}

template <typename C>
void PrintText(const std::string& text, const C& color,
               const cinder::ivec2& size, const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = cinder::TextBox()
                 .alignment(cinder::TextBox::CENTER)
                 .font(cinder::Font(kNormalFont, 30))
                 .size(size)
                 .color(color)
                 .backgroundColor(cinder::ColorA(0, 0, 0, 0))
                 .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}
void MyApp::DrawGameOver() {
  // Lazily print.

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {500, 50};
  const cinder::Color color = cinder::Color::black();
  std::string text = "Game Over :(";
  PrintText(text, color, size, center);
}
void MyApp::DrawMyScore() const {
  const std::string text = "Score: ";
  const cinder::Color color = {1, 1, 0};
  const cinder::ivec2 size = {150, 50};
  const cinder::vec2 loc = {675, 50};

  PrintText(text, color, size, loc);
}

void MyApp::DrawTime() const {
  const std::string text = "Time: " + std::to_string(timer_.getSeconds());
  const cinder::Color color = {1, 1, 0};
  const cinder::ivec2 size = {225, 50};
  const cinder::vec2 loc = {430, 50};

  PrintText(text, color, size, loc);
}

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP: {
      if (player_.GetY() - 40 >= 0) {
        player_.ChangeY(-10);
      }
      break;
    }
    case KeyEvent::KEY_DOWN: {
      if (player_.GetY() + 40 <= getWindowHeight()) {
        player_.ChangeY(10);
      }
      break;
    }
    case KeyEvent::KEY_SPACE: {
      AddBullet(player_.GetX() + 40, player_.GetY(), false);
      break;
    }
  }
}

}  // namespace myapp
