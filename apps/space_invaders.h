// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <cinder/gl/gl.h>
#include <spaceinvaderslibrary/invader.h>
#include <spaceinvaderslibrary/leaderboard.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <string>

#include "cinder/app/RendererGl.h"

using cinder::audio::VoiceRef;

namespace spaceinvaders {

enum class GameState {
  kStart,
  kPlaying,
  kGameOver,
};

class SpaceInvaders : public cinder::app::App {
 public:
  SpaceInvaders();
  void setup() override;
  void update() override;
  void draw() override;
  void AddMissile(const cinder::vec2 &pos);
  void AddPlayer();
  void AddInvader();
  void AddShield();
  void AddShot();
  void DrawShield();
  void DrawMissile();
  void DrawInvader();
  void DrawInvaderShot();
  void DrawAnimation();
  void DrawGameOver();
  void CheckMissileHitInvader(b2Contact* contact);
  void CheckShieldDestroyed(b2Contact* contact);
  void CheckInvaderShot(b2Contact* contact);
  void keyDown(cinder::app::KeyEvent) override;
  void DrawScore();
  void ResetGame();

 private:
  spaceinvaderslibrary::LeaderBoard leaderboard_;
  std::vector<spaceinvaderslibrary::Player> top_players_;
  std::vector<spaceinvaderslibrary::Player> player_scores_;

  int player_x_;
  int player_y_;
  b2World* world_;

  std::vector<b2Body*> missiles_;
  std::vector<b2Body*> invaders_;
  std::vector<b2Body*> shields_;
  std::vector<b2Body*> front_invaders_;
  std::vector<b2Body*> invaders_shots_;

  cinder::gl::Texture2dRef player_texture_;
  cinder::gl::Texture2dRef invader_texture_;
  cinder::gl::Texture2dRef shield_texture_;
  cinder::gl::Texture2dRef fire_texture_;

  std::chrono::time_point<std::chrono::system_clock> animation_time_elapsed_;
  std::chrono::time_point<std::chrono::system_clock> shot_elapsed_;

  cinder::audio::VoiceRef invader_killed_voice_;
  cinder::audio::VoiceRef fire_voice_;

  bool is_destroyed_;
  int animation_x_;
  int animation_y_;
  size_t score_;
  const float kRadius;
  const float kPlayerSize;
  const float kInvaderSize;
  GameState state_;
  std::string player_name_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
