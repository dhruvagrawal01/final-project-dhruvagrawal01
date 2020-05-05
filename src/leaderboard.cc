//
// Created by Dhruv Agrawal on 4/21/20.
//

#include <spaceimpact/leaderboard.h>
#include <spaceimpact/player.h>
#include <sqlite_modern_cpp.h>

#include <string>
#include <vector>

namespace spaceimpact {

using std::string;
using std::vector;

LeaderBoard::LeaderBoard(const string& db_path) : db_{db_path} {
  db_ << "CREATE TABLE if not exists leaderboard (\n"
         "  name  TEXT NOT NULL,\n"
         "  score INTEGER NOT NULL,\n"
         "  time REAL NOT NULL\n"
         ");";
}

void LeaderBoard::AddScoreToLeaderBoard(const Player& player) {
  db_ << "insert into leaderboard (name,score,time) values (?,?,?);"
      << player.name_ << player.score_ << player.time_;
}

vector<Player> GetPlayers(sqlite::database_binder* rows) {
  vector<Player> players;

  for (auto&& row : *rows) {
    string name;
    size_t score;
    double time;
    row >> name >> score >> time;
    Player player = {name, score, time};
    players.push_back(player);
  }

  return players;
}

vector<Player> LeaderBoard::RetrieveHighScores(const size_t limit) {
  auto rows =
      db_ << "select name,score from leaderboard order by score desc limit (?);"
          << limit;
  return GetPlayers(&rows);
}

vector<Player> LeaderBoard::RetrieveHighScores(const Player& player,
                                               const size_t limit) {
  auto rows =
      db_ << "select name,score from leaderboard where name = (?) order by "
             "score desc limit (?);"
          << player.name_ << limit;
  return GetPlayers(&rows);
}

}  // namespace spaceimpact