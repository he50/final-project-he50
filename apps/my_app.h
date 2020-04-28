// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <cinder/audio/audio.h>
#include "cinder/app/RendererGl.h"
#include <mylibrary/leaderboard.h>
#include <mylibrary/invader.h>

using cinder::audio::VoiceRef;

namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void AddMissile(const cinder::vec2 &pos);
  void AddPlayer();
  void addAlien();
  void addShield();
  void keyDown(cinder::app::KeyEvent) override;
  void DrawScore();

 private:
  mylibrary::LeaderBoard leaderboard_;
  int player_x_;
  int player_y_;
  b2World* world_;
  std::vector<b2Body*> missiles_;
  cinder::gl::Texture2dRef fire_texture_;
  cinder::audio::VoiceRef fire_voice_;
  cinder::gl::Texture2dRef player_texture_;
  int kRadius;
  const float kBoxSize = 10;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
