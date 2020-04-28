// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>

namespace myapp {

using cinder::app::KeyEvent;

const char kDbPath[] = "finalproject.db";
const char kNormalFont[] = "Arial";
const float kRadius = 3;

MyApp::MyApp()
    : leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      ship_texture_{
          cinder::gl::Texture2d::create(loadImage(loadAsset("ship.png")))},
      alien_texture_{
          cinder::gl::Texture2d::create(loadImage(loadAsset("alien.png")))},
      shield_texture_{
          cinder::gl::Texture2d::create(loadImage(loadAsset("shield.png")))} {}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();
  b2Vec2 gravity(0.0f, 0.0f);
  mWorld_ = new b2World(gravity);
  timer_.start();
  AddShield();
  player_.x = 50;
  player_.y = 450;
  // player_.SetBody(mWorld_);
}

void MyApp::AddBullet(int a, int b) {
  // Taken from:
  // https://github.com/cinder/Cinder/blob/master/blocks/Box2D/templates
  // /Basic%20Box2D/src/_TBOX_PREFIX_App.cpp

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(a, b);

  b2Body* body = mWorld_->CreateBody(&bodyDef);

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
  cinder::gl::draw(ship_texture_, drawRect);
  cinder::gl::popModelMatrix();
}

void MyApp::AddAlien() {
  for (int i = 0; i < 10; i++) {
    for (int j = 1; j < 5; j++) {
      mylibrary::Alien alien =
          mylibrary::Alien(mWorld_, getWindowWidth() - 70 * j, i * 70 + 100);
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
  for (int i = 0; i < 10; ++i) {
    mWorld_->Step(1 / 30.0f, 10, 10);
  }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  DrawMyScore();
  DrawTime();

  AddShip();

  if (mAliens_.empty()) {
    AddAlien();
  }

  //    if (aliens_.empty()) {
  //      AddAlien();
  //    }

  for (const auto& bullet : mBullets_) {
    cinder::gl::color(1, 0, 0);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(bullet->GetPosition().x, bullet->GetPosition().y);
    // cinder::gl::translate(player_.GetBody()->GetPosition().x,
    // player_.GetBody()->GetPosition().y);
    cinder::gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);
    bullet->SetLinearVelocity(b2Vec2(40.0f, 0.0f));
    cinder::gl::popModelMatrix();
  }

  for (const auto& alien : mAliens_) {
    cinder::gl::color(1, 1, 1);

    cinder::gl::pushModelMatrix();
    // cinder::gl::translate(alien.x, alien.y);
    cinder::gl::translate(alien->GetPosition().x, alien->GetPosition().y);
    // cinder::gl::translate(alien.x, alien.y);
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
      //      if (player_.location.Col() - 40 >= 0) {
      //        player_.location.ChangeCol(-10);
      //      }
      if (player_.y - 40 >= 0) {
        player_.y -= 10;
      }
      break;
    }
    case KeyEvent::KEY_DOWN: {
      //      if (player_.location.Col() + 40 <= getWindowHeight()) {
      //        player_.location.ChangeCol(10);
      //      }
      if (player_.y + 40 <= getWindowHeight()) {
        player_.y += 10;
      }
      break;
    }
    case KeyEvent::KEY_SPACE: {
      // AddBullet(player_.location.Row(), player_.location.Col());
      AddBullet(player_.x + 40, player_.y);
      break;
    }
  }
}

}  // namespace myapp
