//
// Created by Jonathan He on 4/21/20.
//

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <string>
#include <Box2D/Box2D.h>

namespace mylibrary {

class Player {
 public:
  Player() = default;
  Player(const std::string& name, size_t score) : name(name), score(score) {}
  void SetBody(b2World *world, int player_x, int player_y);
  b2Body* GetBody();
  std::string name;
  size_t score;
  b2Body* player_body_;
  const float kBoxSize = 20.0f;
};

}  // namespace mylibrary

#endif  // FINALPROJECT_PLAYER_H
