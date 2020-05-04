// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <cinder/Rand.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <cinder/gl/Texture.h>
#include <cinder/gl/gl.h>
#include <gflags/gflags.h>

#include "mylibrary/alien.h"
#include "mylibrary/engine.h"
#include "mylibrary/leaderboard.h"
#include "mylibrary/shield.h"

namespace myapp {

using cinder::audio::SourceFileRef;
using cinder::audio::VoiceRef;

enum class GameState {
  kPlaying,
  kGameOver,
};

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  // Displays a counter of the current score (snake size)
  void DrawMyScore() const;
  // Displays a counter of the time elapsed
  void DrawTime() const;
  // Adds a bullet at the passed position
  //  void AddBullet(int x, int y, bool is_alien);
  void AddShip();
  //  void AddAlien();
  //  void AddShield();
  void DrawGameOver();
  void DrawBullets();
  void DrawAliens();
  void DrawShields();
  void ResetGame();

  void TakeAction(b2Contact *contact);
  // Reset game?
  // Pause?

 private:
  //  b2World *mWorld_;
  const std::string player_name_;
  //  std::vector<b2Body *> mBullets_;
  //  std::vector<b2Body *> mAliens_;
  //  std::vector<b2Body *> first_row_;
  //  std::vector<b2Body *> mShields_;
  std::chrono::time_point<std::chrono::system_clock> last_time_;
  std::vector<mylibrary::Player> top_players_;
  // A vector of Player objects containing the current player's high scores
  std::vector<mylibrary::Player> player_scores_;
  mylibrary::LeaderBoard leaderboard_;
  cinder::Timer timer_;
  mylibrary::Player player_;
  cinder::gl::Texture2dRef ship_texture_;
  cinder::gl::Texture2dRef alien_texture_;
  cinder::gl::Texture2dRef shield_texture_;
  GameState state_;
  // Cinder SourFileRef object with the background music file
  SourceFileRef alien_killed;
  // Cinder SourceFileRef object with the munch noise file
  SourceFileRef player_killed;
  // Cinder VoiceRef object with the background music file
  VoiceRef alien_killed_voice_;
  // Cinder VoiceRef object with the munch noise file
  VoiceRef player_killed_voice_;
  // Cinder Timer object to keep track of the time elapsed
  mylibrary::Engine engine_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
