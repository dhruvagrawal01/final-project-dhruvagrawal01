// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2World.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <cinder/gl/Texture.h>

#include "mylibrary/alien.h"
#include "mylibrary/leaderboard.h"
#include "mylibrary/shield.h"

// class ListenerClass : public b2ContactListener {
//
//  void BeginContact(b2Contact* contact) override {
//    b2Fixture* f1 = contact->GetFixtureA();
//    b2Fixture* f2 = contact->GetFixtureB();
//
//    b2Body* b1 = f1->GetBody();
//    b2Body* b2 = f2->GetBody();
//
//    if (b1->GetType() == b2_dynamicBody && b2->GetType() == b2_dynamicBody) {
//      b1->DestroyFixture(f1);
//      b2->DestroyFixture(f2);
//    }
//  }
//};

namespace myapp {

using cinder::audio::SourceFileRef;
using cinder::audio::VoiceRef;

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  // Displays a counter of the current score (snake size)
  void DrawMyScore() const;
  // Displays a counter of the time elapsed
  void DrawTime() const;
  void keyDown(cinder::app::KeyEvent) override;
  // Adds a bullet at the passed position
  void AddBullet(int x, int y, bool is_alien);
  void AddShip();
  void AddAlien();
  void AddShield();
  void DrawGameOver();

 private:
  std::chrono::time_point<std::chrono::system_clock> last_time_;
  b2World *mWorld_;
  std::vector<b2Body *> mBullets_;
  std::vector<b2Body *> mAliens_;
  std::vector<b2Body *> first_row_;
  std::vector<b2Body *> mShields_;
  mylibrary::LeaderBoard leaderboard_;
  cinder::Timer timer_;
  mylibrary::Player player_;
  cinder::gl::Texture2dRef ship_texture_;
  cinder::gl::Texture2dRef alien_texture_;
  cinder::gl::Texture2dRef shield_texture_;
  int gameState;
  int score_ = 0;
  // Cinder SourFileRef object with the background music file
  SourceFileRef alien_killed;
  // Cinder SourceFileRef object with the munch noise file
  SourceFileRef player_killed;
  // Cinder VoiceRef object with the background music file
  VoiceRef alien_killed_voice_;
  // Cinder VoiceRef object with the munch noise file
  VoiceRef player_killed_voice_;
  // Cinder Timer object to keep track of the time elapsed
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
