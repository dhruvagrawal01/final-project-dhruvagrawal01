// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_SPACEIMPACTAPP_H_
#define FINALPROJECT_APPS_SPACEIMPACTAPP_H_

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

#include "spaceimpact/alien.h"
#include "spaceimpact/engine.h"
#include "spaceimpact/leaderboard.h"
#include "spaceimpact/shield.h"

namespace spaceimpactapp {

using cinder::audio::SourceFileRef;
using cinder::audio::VoiceRef;

/**
 * Enum class representing the state of the game
 */
enum class GameState {
  kPlaying,
  kGameOver,
  kMenu,
};

class SpaceImpactApp : public cinder::app::App {
 public:
  SpaceImpactApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;

  /**
   * Resets all the game variables, starting a new game
   */
  void ResetGame();

  /**
   * Initializes the audio files and textures used for images
   */
  void SetupUtils();

  /**
   * Draws the ship representing the player
   */
  void DrawPlayer() const;

  /**
   * Draws any bullets shot by the player or an alien
   */
  void DrawBullets();

  /**
   * Draws the remaining number of aliens
   */
  void DrawAliens() const;

  /**
   * Draws the remaining number of shields
   */
  void DrawShields() const;

  /**
   * Draws a welcome menu displaying the controls and instructions
   */
  void DrawMenu() const;

  /**
   * Displays the alien wave count
   */
  void DrawWave() const;

  /**
   * Displays the time elapsed
   */
  void DrawTime() const;

  /**
   * Displays a counter of the player's score
   */
  void DrawScore() const;

  /**
   * Draws a game over screen once the player dies
   */
  void DrawGameOver();

  /**
   * Checks whether the player passed the bounds of the game window
   * @return whether the player crossed the bounds
   */
  bool CrossesBoundary();

 private:
  cinder::gl::Texture2dRef bg_image_;
  cinder::gl::Texture2dRef menu_image_;
  cinder::gl::Texture2dRef ship_image_;
  cinder::gl::Texture2dRef alien_image_;
  cinder::gl::Texture2dRef shield_image_;
  VoiceRef bg_music_;
  VoiceRef alien_shot_;
  VoiceRef player_shot_;

  spaceimpact::Engine engine_;
  spaceimpact::Player player_;
  spaceimpact::LeaderBoard leaderboard_;
  const std::string player_name_;
  cinder::Timer timer_;
  GameState state_;
  std::chrono::time_point<std::chrono::system_clock> last_time_;
  std::vector<spaceimpact::Player> top_players_;
  std::vector<spaceimpact::Player> player_scores_;
  // The current alien wave's number
  int num_wave_ = 0;
};

}  // namespace spaceimpactapp

#endif  // FINALPROJECT_APPS_SPACEIMPACTAPP_H_
