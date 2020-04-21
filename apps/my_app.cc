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

MyApp::MyApp() : leaderboard_{cinder::app::getAssetPath(kDbPath).string()} {}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  b2Vec2 gravity(10.0f, 0.0f);
  mWorld_ = new b2World(gravity);

  timer_.start();
}

void MyApp::AddBullet(const cinder::vec2& pos) {
  // Taken from:
  // https://github.com/cinder/Cinder/blob/master/blocks/Box2D/templates
  // /Basic%20Box2D/src/_TBOX_PREFIX_App.cpp
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos.x, pos.y);

  b2Body* body = mWorld_->CreateBody(&bodyDef);

  b2CircleShape bullet;
  bullet.m_p.Set(2.0f, 3.0f);
  bullet.m_radius = kRadius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &bullet;
  fixtureDef.density = 1.0f;

  body->CreateFixture(&fixtureDef);
  mBullets_.push_back(body);
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

  cinder::gl::color(1, 0, 0);

  for (const auto& bullet : mBullets_) {
    cinder::gl::pushModelMatrix();

    cinder::gl::translate(bullet->GetPosition().x, bullet->GetPosition().y);
    cinder::gl::rotate(bullet->GetAngle());
    cinder::gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);

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

void MyApp::keyDown(KeyEvent event) {}

void MyApp::mouseDown(cinder::app::MouseEvent event) {
  AddBullet(event.getPos());
}

}  // namespace myapp
