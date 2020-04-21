// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <Box2D/Dynamics/b2World.h>
#include <cinder/app/App.h>

#include "mylibrary/leaderboard.h"

namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void DrawMyScore() const;
  void DrawTime() const;
  void keyDown(cinder::app::KeyEvent) override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void AddBullet(const cinder::vec2 &pos);
  b2World *mWorld_;
  std::vector<b2Body *> mBullets_;
  mylibrary::LeaderBoard leaderboard_;
  cinder::Timer timer_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
