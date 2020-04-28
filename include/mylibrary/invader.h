//
// Created by Jonathan He on 4/28/20.
//

#ifndef FINALPROJECT_INVADER_H
#define FINALPROJECT_INVADER_H

#include <Box2D/Box2D.h>

namespace mylibrary {

class Invader {
 public:
  Invader(b2World *world, int x_coordinate, int y_coordinate);
  b2Body* GetBody();
 private:
  b2Body* invader_body_;
};

}  // namespace mylibrary

#endif  // FINALPROJECT_INVADER_H
