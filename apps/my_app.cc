// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

namespace myapp {

using namespace std::chrono;

using cinder::app::KeyEvent;
using cinder::audio::Voice;
using cinder::gl::Texture2d;
using std::chrono::system_clock;

const char kDbPath[] = "finalproject.db";
const char kNormalFont[] = "Arial";
const size_t kLimit = 3;
const float kRadius = 3;
const char kAlienDead[] = "invaderkilled.wav";
const char kPlayerDead[] = "explosion.wav";

DECLARE_string(name);

MyApp::MyApp()
    : player_name_{FLAGS_name},
      leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      ship_texture_{Texture2d::create(loadImage(loadAsset("ship.png")))},
      alien_texture_{Texture2d::create(loadImage(loadAsset("alien.png")))},
      shield_texture_{Texture2d::create(loadImage(loadAsset("shield.png")))},
      alien_killed{cinder::audio::load(cinder::app::loadAsset(kAlienDead))},
      player_killed{cinder::audio::load(cinder::app::loadAsset(kPlayerDead))},
      alien_killed_voice_{Voice::create(alien_killed)},
      player_killed_voice_{Voice::create(player_killed)},
      engine_(getWindowWidth(), getWindowHeight()),
      state_{GameState::kPlaying} {}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();
  player_.name = player_name_;
  timer_.start();
  engine_.AddShield();
  cinder::Rand::randomize();
  player_.SetBody(engine_.mWorld_);
}

void MyApp::update() {
  if (state_ == GameState::kGameOver) {
    if (top_players_.empty()) {
      leaderboard_.AddScoreToLeaderBoard(player_);
      top_players_ = leaderboard_.RetrieveHighScores(kLimit);

      // It is crucial the this vector be populated, given that `kLimit` > 0.
      assert(!top_players_.empty());
    }
    // To populate the vector containing the high scores of current player
    if (player_scores_.empty()) {
      player_scores_ = leaderboard_.RetrieveHighScores(player_, kLimit);

      // It is crucial the this vector be populated, given that `kLimit` > 0.
      assert(!player_scores_.empty());
    }
    return;
  }

  if (engine_.GetAliens().empty()) {
    engine_.AddAlien();
  }

  const auto time = system_clock::now();
  long double time_since_change =
      duration_cast<milliseconds>(time - last_time_).count() / 1000.0;

  if (timer_.getSeconds() > 2.5 && time_since_change >= 3.0 &&
      !engine_.GetFirstRow().empty()) {
    int rand_body = cinder::Rand::randInt(0, engine_.GetFirstRow().size());
    b2Body* alien = engine_.GetFirstRow().at(rand_body);
    engine_.AddBullet(alien->GetPosition().x - 25, alien->GetPosition().y,
                      true);
    last_time_ = time;
  }

  for (int i = 0; i < 10; ++i) {
    engine_.mWorld_->Step(1 / 30.0f, 10, 10);

    mylibrary::ResultantAction action = engine_.Step();
    if (action == mylibrary::ResultantAction::AlienKilled) {
      alien_killed_voice_->setVolume(0.1);
      alien_killed_voice_->start();
      player_.score += 25;
    } else if (action == mylibrary::ResultantAction::PlayerKilled) {
      state_ = GameState::kGameOver;
      player_.time = timer_.getSeconds();
      // stop timer?
      player_killed_voice_->setVolume(0.1);
      player_killed_voice_->start();
    }
  }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  if (state_ == GameState::kGameOver) {
    cinder::gl::clear(cinder::Color(1, 0, 0));
    DrawGameOver();
    return;
  }

  cinder::gl::clear();
  // cinder::gl::clear(cinder::Color(0.8, 1, 0.89));

  DrawMyScore();
  DrawTime();

  AddShip();

  DrawBullets();
  DrawAliens();
  DrawShields();
}

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP: {
      if (player_.GetBody()->GetPosition().y - 40 >= 0) {
        // player_.ChangeY(-10);
        player_.GetBody()->SetLinearVelocity({0, -5.0f});
      }
      break;
    }
    case KeyEvent::KEY_DOWN: {
      if (player_.GetBody()->GetPosition().y <= getWindowHeight()) {
        // player_.ChangeY(10);
        player_.GetBody()->SetLinearVelocity({0, 5.0f});
      }
      break;
    }
    case KeyEvent::KEY_SPACE: {
      //      engine_.AddBullet(player_.GetX() + 40, player_.GetY(), false);
      engine_.AddBullet(player_.GetBody()->GetPosition().x + 40,
                        player_.GetBody()->GetPosition().y, false);
      break;
    }
    case KeyEvent::KEY_r: {
      ResetGame();
      break;
    }
  }
}

void MyApp::ResetGame() {
  state_ = GameState::kPlaying;
  top_players_.clear();
  player_scores_.clear();
  player_.Reset();
  engine_.Reset();
  player_.SetBody(engine_.mWorld_);
  timer_.start();
  engine_.AddShield();
  cinder::Rand::randomize();
}

void MyApp::DrawBullets() {
  for (const auto& bullet : engine_.GetBullets()) {
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
      //      engine_.GetBullets().erase(std::remove(engine_.GetBullets().begin(),
      //                                          engine_.GetBullets().end(),
      //                                          bullet),
      //                              engine_.GetBullets().end());
      engine_.RemoveBullet(bullet);
    }
    cinder::gl::popModelMatrix();
  }
}

void MyApp::DrawAliens() {
  for (const auto& alien : engine_.GetAliens()) {
    cinder::gl::color(1, 1, 1);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(alien->GetPosition().x, alien->GetPosition().y);
    cinder::Rectf drawRect(-20, -20, 20, 20);
    cinder::gl::draw(alien_texture_, drawRect);
    cinder::gl::popModelMatrix();
  }
}

void MyApp::DrawShields() {
  for (const auto& shield : engine_.GetShields()) {
    cinder::gl::color(0, 1, 0);

    cinder::gl::pushModelMatrix();
    cinder::gl::translate(shield->GetPosition().x, shield->GetPosition().y);
    cinder::Rectf drawRect(-20, -30, 20, 30);
    cinder::gl::draw(shield_texture_, drawRect);
    cinder::gl::popModelMatrix();
  }
}

void MyApp::AddShip() {
  cinder::gl::color(0.68, 0.68, 0.68);
  cinder::gl::pushModelMatrix();
  cinder::gl::translate(player_.GetBody()->GetPosition().x,
                        player_.GetBody()->GetPosition().y);

  cinder::Rectf drawRect(-40, -40, 40, 40);

  cinder::gl::draw(ship_texture_, drawRect);
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

void MyApp::DrawGameOver() {
  // Lazily print.
  if (top_players_.empty()) return;
  if (player_scores_.empty()) return;

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {500, 50};
  const cinder::Color color = cinder::Color::black();

  size_t row = 0;
  PrintText("Game Over :(", color, size, center);
  for (const mylibrary::Player& player : top_players_) {
    std::stringstream ss;
    ss << player.name << " - " << player.score;
    PrintText(ss.str(), color, size, {center.x, center.y + (++row) * 50});
  }

  size_t score_count = 0;
  std::string player_name = player_scores_.at(0).name;

  for (const mylibrary::Player& player : player_scores_) {
    std::stringstream ss;
    ss << "High score " << ++score_count << " for " << player_name << " - ";
    ss << player.score;
    PrintText(ss.str(), color, size, {center.x, center.y + (++row) * 50});
  }
}

void MyApp::DrawMyScore() const {
  const std::string text = "Score: " + std::to_string(player_.score);
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

}  // namespace myapp
