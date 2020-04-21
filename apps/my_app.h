// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>

#include "cinder/app/RendererGl.h"

namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void addBox(const cinder::vec2 &pos);
  void keyDown(cinder::app::KeyEvent) override;

};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
