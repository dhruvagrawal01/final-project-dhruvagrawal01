//
// Created by Dhruv Agrawal on 4/21/20.
//

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <string>

namespace mylibrary {

struct Player {
  Player(const std::string& name, size_t score, double time)
      : name(name), score(score), time(time) {}
  std::string name;
  size_t score;
  double time;
};

}  // namespace mylibrary

#endif  // FINALPROJECT_PLAYER_H
