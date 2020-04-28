//
// Created by Jonathan He on 4/28/20.
//

#ifndef FINALPROJECT_SHIELD_H
#define FINALPROJECT_SHIELD_H

#include <Box2D/Box2D.h>

namespace mylibrary {

class Shield {
 public:
  Shield(b2World *world, int x_coordinate, int y_coordinate);
  b2Body* GetBody();
 private:
  size_t x;
  size_t y;
  b2Body* shield_body_;
};

}  // namespace mylibrary

#endif  // FINALPROJECT_SHIELD_H
