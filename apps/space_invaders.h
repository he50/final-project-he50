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

/**
 * Enum values for the different game states.
 * kStart is set when the game starts.
 * kPlaying is set once the player starts firing.
 * kGameOver is set once the game is over: alien shot hits player or
 * player defeats all invaders.
 */
enum class GameState {
  kStart,
  kPlaying,
  kGameOver,
};

/**
 * SpaceInvaders class for all of game functionality.
 */
class SpaceInvaders : public cinder::app::App {
 public:
  /**
   * Default Constructor.
   */
  SpaceInvaders();

  /**
   * Function to setup the game.
   * Initializes b2World.
   */
  void setup() override;

  /**
   * Function to update the game.
   * Checks for all possible collisions.
   */
  void update() override;

  /**
   * Function to draw the game.
   * Draws player, invaders, shield and invader shots.
   * Considers removal from invader shots and player missiles.
   */
  void draw() override;

  /**
   * Function that adds missile on space key press.
   * @param pos vec2 position on where the player presses space.
   */
  void AddMissile(const cinder::vec2 &pos);

  /**
   * Adds player to the cinder GUI.
   */
  void AddPlayer();

  /**
   * Adds invader to vector at the start of the game.
   */
  void AddInvader();

  /**
   * Adds shield to vector at the start of the game.
   */
  void AddShield();

  /**
   * Adds random shot to vector every 1 second.
   */
  void AddShot();

  /**
   * Draws shields from shield vector.
   */
  void DrawShield();

  /**
   * Draws missiles from missile vector.
   */
  void DrawMissile();

  /**
   * Draws invaders from invader vector.
   */
  void DrawInvader();

  /**
   * Draws shots from invader shot vector.
   */
  void DrawInvaderShot();

  /**
   * Draws animation using the animation_x_ and animation_y_ variable.
   */
  void DrawAnimation();

  /**
   * Draws Game over screen once conditions have been reached.
   */
  void DrawGameOver();

  /**
   * Checks if missile hit invader given the contact variable.
   * @param contact b2Contact object. Has two components Fixture A and B.
   */
  void CheckMissileHitInvader(b2Contact* contact);

  /**
   * Checks if shield is destroyed given the contact variable.
   * @param contact b2Contact object. Has two components Fixture A and B.
   */
  void CheckShieldDestroyed(b2Contact* contact);

  /**
   * Checks if invader shot hit given the contact variable.
   * @param contact b2Contact object. Has two components Fixture A and B.
   */
  void CheckInvaderShot(b2Contact* contact);

  /**
   * Key function that gets called for a given key press.
   * R - resets the game
   * ESC - quits the game
   * Left - player moves left
   * Right - player moves right
   * SPACE - shoot
   */
  void keyDown(cinder::app::KeyEvent) override;

  /**
   * Draws score at the top of the screen.
   */
  void DrawScore();

  /**
   * Function called by keyDown once R gets pressed.
   * Resets all state variables.
   */
  void ResetGame();

 private:
  /** Leaderboard object from leaderboard class. **/
  spaceinvaderslibrary::LeaderBoard leaderboard_;
  /** Vector of Players for score tracking. **/
  std::vector<spaceinvaderslibrary::Player> top_players_;
  std::vector<spaceinvaderslibrary::Player> player_scores_;

  /** X and Y Coordinates for the player's position.**/
  int player_x_;
  int player_y_;

  /** World representing the Box2D world. **/
  b2World* world_;

  /** The b2Body vectors for drawing. **/
  std::vector<b2Body*> missiles_;
  std::vector<b2Body*> invaders_;
  std::vector<b2Body*> shields_;
  std::vector<b2Body*> front_invaders_;
  std::vector<b2Body*> invaders_shots_;

  /** Textures for images. **/
  cinder::gl::Texture2dRef player_texture_;
  cinder::gl::Texture2dRef invader_texture_;
  cinder::gl::Texture2dRef shield_texture_;
  cinder::gl::Texture2dRef fire_texture_;

  /** Time variables for animation and random invader shots. **/
  std::chrono::time_point<std::chrono::system_clock> animation_time_elapsed_;
  std::chrono::time_point<std::chrono::system_clock> shot_elapsed_;

  /** VoiceRef objects for sound. **/
  cinder::audio::VoiceRef invader_killed_voice_;
  cinder::audio::VoiceRef fire_voice_;
  cinder::audio::VoiceRef background_voice_;
  cinder::audio::VoiceRef explosion_voice_;

  /** Boolean to represent whether an invader is destroyed. For animation. **/
  bool is_destroyed_;

  /** X and Y coordinates for animation. **/
  int animation_x_;
  int animation_y_;

  /** Score variable: 20 points per invader taken down. **/
  size_t score_;

  /** Constants. **/
  const float kRadius;
  const float kPlayerSize;
  const float kInvaderSize;
  const float kVelocity;

  /** GameState variable for certain functions. **/
  GameState state_;

  /** Player name string value from gflags. **/
  std::string player_name_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
