// Copyright (c) 2020 [Dhruv Agrawal]. All rights reserved.

#include "space_impact_app.h"

namespace spaceimpactapp {

using namespace std::chrono;

using cinder::app::KeyEvent;
using cinder::audio::Voice;
using cinder::gl::Texture2d;
using std::chrono::system_clock;

const char kDbPath[] = "finalproject.db";
const char kNormalFont[] = "Arial";
const char kBGMusic[] = "bgmusic.mp3";
const char kAlienDead[] = "invaderkilled.wav";
const char kPlayerDead[] = "explosion.wav";
const char kBGImage[] = "space.jpg";
const char kMenuImage[] = "welcome.jpg";
const char kShipImage[] = "ship.png";
const char kAlienImage[] = "alien.png";
const char kShieldImage[] = "shield.png";
const size_t kLimit = 3;
const float kRadius = 3.0f;
const double kShootRate = 2;
const int kScoreIncrement = 25;
const float kStartSpeed = 5.0f;
const float kBulletSpeed = 40.0f;
const int kAlienSize = 20;
const int kShieldSize = 25;
const int kShipSize = 40;

DECLARE_string(name);

SpaceImpactApp::SpaceImpactApp()
    : player_name_{FLAGS_name},
      leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      engine_(getWindowWidth(), getWindowHeight()),
      state_{GameState::kMenu} {}

void SpaceImpactApp::setup() {
  cinder::Rand::randomize();
  SetupUtils();
  player_.name_ = player_name_;
  player_.SetBody(engine_.GetWorld());
  engine_.AddShield();
  bg_music_->setVolume(10);
  bg_music_->start();
}

void SpaceImpactApp::update() {
  if (state_ == GameState::kGameOver) {
    // To populate the vector containing the overall high scores
    if (top_players_.empty()) {
      leaderboard_.AddScoreToLeaderBoard(player_);
      top_players_ = leaderboard_.RetrieveHighScores(kLimit);
      assert(!top_players_.empty());
    }
    // To populate the vector containing the high scores of current player
    if (player_scores_.empty()) {
      player_scores_ = leaderboard_.RetrieveHighScores(player_, kLimit);
      assert(!player_scores_.empty());
    }
    bg_music_->stop();
    return;
  }

  if (!bg_music_->isPlaying()) {
    bg_music_->start();
  }

  if (state_ == GameState::kMenu) {
    return;
  }

  // To add a new wave of aliens once an entire wave is killed
  if (engine_.GetAliens().empty()) {
    engine_.AddAlien();
    num_wave_++;
  }

  const auto time = system_clock::now();
  long double time_since_change =
      duration_cast<milliseconds>(time - last_time_).count() / 1000.0;

  // To make a random alien in the first row shoot every kShootRate seconds
  if (timer_.getSeconds() > kShootRate && time_since_change >= kShootRate &&
      !engine_.GetFirstRow().empty()) {
    int rand_alien = cinder::Rand::randInt(0, engine_.GetFirstRow().size());
    b2Body* alien = engine_.GetFirstRow().at(rand_alien);
    engine_.AddBullet(alien->GetPosition().x - kAlienSize,
                      alien->GetPosition().y, true);
    last_time_ = time;
  }

  for (int i = 0; i < 10; ++i) {
    engine_.GetWorld()->Step(1 / 30.0f, 10, 10);
    spaceimpact::ResultantAction action = engine_.Step();

    if (action == spaceimpact::ResultantAction::AlienKilled) {
      alien_shot_->setVolume(0.5);
      alien_shot_->start();
      player_.score_ += kScoreIncrement;
    } else if (action == spaceimpact::ResultantAction::PlayerKilled ||
               CrossesBoundary()) {
      state_ = GameState::kGameOver;
      timer_.stop();
      player_.time_ = timer_.getSeconds();
      player_shot_->setVolume(0.5);
      player_shot_->start();
    }
  }
}

void SpaceImpactApp::draw() {
  cinder::gl::enableAlphaBlending();
  if (state_ == GameState::kGameOver) {
    cinder::gl::clear(cinder::Color(1, 0, 0));
    DrawGameOver();
    return;
  }

  cinder::gl::clear();
  cinder::gl::color(0.7, 0.7, 1);
  cinder::gl::draw(bg_image_);

  if (state_ == GameState::kMenu) {
    DrawMenu();
    return;
  }

  DrawScore();
  DrawTime();
  DrawWave();

  DrawPlayer();
  DrawBullets();
  DrawAliens();
  DrawShields();
}

void SpaceImpactApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP: {
      // Sets upward velocity according to the alien wave count
      player_.GetBody()->SetLinearVelocity({0, -kStartSpeed * num_wave_});
      break;
    }
    case KeyEvent::KEY_DOWN: {
      // Sets downward velocity according to the alien wave count
      player_.GetBody()->SetLinearVelocity({0, kStartSpeed * num_wave_});
      break;
    }
    case KeyEvent::KEY_SPACE: {
      if (state_ == GameState::kPlaying) {
        // Adds a bullet originating from the player
        engine_.AddBullet(player_.GetBody()->GetPosition().x + kShipSize,
                          player_.GetBody()->GetPosition().y, false);
      }
      break;
    }
    case KeyEvent::KEY_r: {
      ResetGame();
      break;
    }
    case KeyEvent::KEY_RETURN: {
      if (state_ == GameState::kMenu) {
        state_ = GameState::kPlaying;
        timer_.start();
      }
      break;
    }
  }
}

void SpaceImpactApp::ResetGame() {
  state_ = GameState::kPlaying;
  top_players_.clear();
  player_scores_.clear();
  player_.Reset();
  engine_.Reset();
  player_.SetBody(engine_.GetWorld());
  // Needs to be stopped to replay music from beginning
  bg_music_->stop();
  bg_music_->start();
  timer_.start();
  engine_.AddShield();
  num_wave_ = 0;
}

void SpaceImpactApp::SetupUtils() {
  bg_image_ = Texture2d::create(loadImage(loadAsset(kBGImage)));
  menu_image_ = Texture2d::create(loadImage(loadAsset(kMenuImage)));
  ship_image_ = Texture2d::create(loadImage(loadAsset(kShipImage)));
  alien_image_ = Texture2d::create(loadImage(loadAsset(kAlienImage)));
  shield_image_ = Texture2d::create(loadImage(loadAsset(kShieldImage)));

  SourceFileRef bg_music_file =
      cinder::audio::load(cinder::app::loadAsset(kBGMusic));
  bg_music_ = Voice::create(bg_music_file);

  SourceFileRef alien_shot_file =
      cinder::audio::load(cinder::app::loadAsset(kAlienDead));
  alien_shot_ = Voice::create(alien_shot_file);

  SourceFileRef player_shot_file =
      cinder::audio::load(cinder::app::loadAsset(kPlayerDead));
  player_shot_ = Voice::create(player_shot_file);
}

void SpaceImpactApp::DrawBullets() {
  for (const auto& bullet : engine_.GetBullets()) {
    cinder::gl::color(1, 0, 0);
    cinder::gl::pushModelMatrix();
    cinder::gl::translate(bullet->GetPosition().x, bullet->GetPosition().y);
    cinder::gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);

    if (bullet->GetUserData()) {
      // Bullet is shot by an alien
      bullet->SetLinearVelocity(b2Vec2(-kBulletSpeed, 0.0f));
    } else {
      // Bullet is shot by the player
      bullet->SetLinearVelocity(b2Vec2(kBulletSpeed, 0.0f));
    }

    // To avoid drawing a bullet once it crosses the screen bounds
    if (bullet->GetPosition().x > getWindowWidth() ||
        bullet->GetPosition().x < 0) {
      engine_.RemoveBullet(bullet);
    }
    cinder::gl::popModelMatrix();
  }
}

void SpaceImpactApp::DrawAliens() const {
  for (const auto& alien : engine_.GetAliens()) {
    cinder::gl::color(1, 1, 1);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(alien->GetPosition().x, alien->GetPosition().y);
    cinder::Rectf drawRect(-kAlienSize, -kAlienSize, kAlienSize, kAlienSize);
    cinder::gl::draw(alien_image_, drawRect);
    cinder::gl::popModelMatrix();
  }
}

void SpaceImpactApp::DrawShields() const {
  for (const auto& shield : engine_.GetShields()) {
    cinder::gl::color(0, 1, 0);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(shield->GetPosition().x, shield->GetPosition().y);
    cinder::Rectf drawRect(-kShieldSize, -kShieldSize, kShieldSize,
                           kShieldSize);
    cinder::gl::draw(shield_image_, drawRect);
    cinder::gl::popModelMatrix();
  }
}

void SpaceImpactApp::DrawPlayer() const {
  cinder::gl::color(0.68, 0.68, 0.68);
  cinder::gl::pushModelMatrix();
  cinder::gl::translate(player_.GetBody()->GetPosition().x,
                        player_.GetBody()->GetPosition().y);

  cinder::Rectf drawRect(-kShipSize, -kShipSize, kShipSize, kShipSize);

  cinder::gl::draw(ship_image_, drawRect);
  cinder::gl::popModelMatrix();
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

void SpaceImpactApp::DrawGameOver() {
  if (top_players_.empty()) return;
  if (player_scores_.empty()) return;

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {500, 50};
  const cinder::Color color = cinder::Color::black();

  size_t row = 0;
  PrintText("Oops, you died :(", color, size, center);

  for (const spaceimpact::Player& player : top_players_) {
    std::stringstream ss;
    ss << player.name_ << " - " << player.score_;
    PrintText(ss.str(), color, size, {center.x, center.y + (++row) * 50});
  }

  size_t score_count = 0;
  std::string player_name = player_scores_.at(0).name_;

  for (const spaceimpact::Player& player : player_scores_) {
    std::stringstream ss;
    ss << "High score " << ++score_count << " for " << player_name << " - ";
    ss << player.score_;
    PrintText(ss.str(), color, size, {center.x, center.y + (++row) * 50});
  }
}

void SpaceImpactApp::DrawMenu() const {
  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {550, 500};
  cinder::Color color = {1, 1, 0};

  std::stringstream ss;
  ss << player_name_ << ", welcome to Space Impact!\n\n";
  PrintText(ss.str(), color, size, {center.x, center.y - 50});
  ss.clear();
  ss.str(std::string());
  color = cinder::Color::white();
  ss << "Instructions:\n";
  ss << "Avoid the aliens' bullets\n";
  ss << "Don't go past the screen bounds\n";
  ss << "Press ENTER to start!\n\n";
  ss << "Controls:\n";
  ss << "UP, DOWN : Move\n";
  ss << "SPACE : Shoot\n";
  ss << "r : Reset\n";
  PrintText(ss.str(), color, size, {center.x, center.y});

  cinder::gl::color(0, 1, 0);
  cinder::gl::pushModelMatrix();
  cinder::gl::translate({getWindowCenter().x, getWindowCenter().y + 225});
  cinder::Rectf drawRect(-200, -80, 200, 80);
  cinder::gl::draw(menu_image_, drawRect);
  cinder::gl::popModelMatrix();
}

void SpaceImpactApp::DrawScore() const {
  const std::string text = "Score: " + std::to_string(player_.score_);
  const cinder::Color color = {1, 1, 0};
  const cinder::ivec2 size = {200, 50};
  const cinder::vec2 loc = {650, 50};

  PrintText(text, color, size, loc);
}

void SpaceImpactApp::DrawTime() const {
  const std::string text = "Time: " + std::to_string(timer_.getSeconds());
  const cinder::Color color = {1, 1, 0};
  const cinder::ivec2 size = {275, 50};
  const cinder::vec2 loc = {400, 50};

  PrintText(text, color, size, loc);
}

void SpaceImpactApp::DrawWave() const {
  const std::string text = "Wave: " + std::to_string(num_wave_);
  const cinder::Color color = {1, 1, 0};
  const cinder::ivec2 size = {150, 50};
  const cinder::vec2 loc = {125, 50};

  PrintText(text, color, size, loc);
}

bool SpaceImpactApp::CrossesBoundary() {
  return player_.GetBody()->GetPosition().y <= 0 ||
         player_.GetBody()->GetPosition().y >= getWindowHeight();
}

}  // namespace spaceimpactapp
