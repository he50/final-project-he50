// Copyright (c) 2020 [Your Name]. All rights reserved.
#include "space_invaders.h"

#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <spaceinvaderslibrary/invader.h>
#include <spaceinvaderslibrary/player.h>
#include <spaceinvaderslibrary/shield.h>
#include <gflags/gflags.h>

#include <vector>

#include "cinder/app/RendererGl.h"
#include "cinder/audio/audio.h"

const char kDbPath[] = "spaceinvaders.db";
const char kNormalFont[] = "Arial";
using cinder::audio::VoiceRef;

DECLARE_string(name);

namespace spaceinvaders {

using cinder::app::KeyEvent;
using namespace ci::app;

SpaceInvaders::SpaceInvaders()
    : leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      player_y_{getWindowHeight() - 30},
      player_x_{getWindowWidth() / 2},
      kPlayerSize{15},
      kRadius{3},
      kInvaderSize{18},
      kVelocity{30},
      kInvaderRows{4},
      kInvaderCols{11},
      kWaitTime{1.0},
      kLimit{3},
      kMaxScore{880},
      player_name_{FLAGS_name},
      score_{0},
      state_{GameState::kStart} {}

void SpaceInvaders::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  b2Vec2 gravity(0.0f, 0.0f);
  world_ = new b2World(gravity);

  player_texture_ = cinder::gl::Texture2d::create(
      loadImage(loadAsset("player.png")));
  invader_texture_ = cinder::gl::Texture2d::create(
      loadImage(loadAsset("invader.png")));
  shield_texture_ = cinder::gl::Texture2d::create(
      loadImage(loadAsset("shield.png")));
  fire_texture_ = cinder::gl::Texture2d::create(
      loadImage(loadAsset("fire.png")));

  cinder::audio::SourceFileRef bgm_file =
      cinder::audio::load
      (cinder::app::loadAsset("background_music.mp3"));
  background_voice_ = cinder::audio::Voice::create(bgm_file);

  cinder::audio::SourceFileRef explosion_file =
      cinder::audio::load(
          cinder::app::loadAsset("explosion.wav"));
  explosion_voice_ = cinder::audio::Voice::create(explosion_file);

  // Start playing audio from the voice:
  background_voice_->start();
}

template <typename C>
void PrintText(const std::string& text, const C& color,
               const cinder::ivec2& size, const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 30))
      .size(size)
      .color(color)
      .backgroundColor(cinder::ColorA(1,0,0,0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x/2, loc.y -box_size.y/2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void SpaceInvaders::AddMissile(const cinder::vec2 &pos) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos.x, pos.y - kPlayerSize);

  b2Body *body = world_->CreateBody(&bodyDef);

  b2CircleShape bullet;
  bullet.m_p.Set(kRadius, kRadius);
  bullet.m_radius = kRadius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &bullet;
  fixtureDef.density = 1.0f;

  body->CreateFixture(&fixtureDef);
  body->SetUserData((void*)"missile");
  missiles_.push_back(body);
}

void SpaceInvaders::AddPlayer() {
  spaceinvaderslibrary::Player player = spaceinvaderslibrary::Player();
  player.SetBody(world_, player_x_, player_y_);

  cinder::gl::pushModelMatrix();
  cinder::gl::translate(player.GetBody()->GetPosition().x,
                player.GetBody()->GetPosition().y);
  cinder::gl::rotate(player.GetBody() ->GetAngle());
  cinder::Rectf rectangle = cinder::Rectf(-kPlayerSize, -kPlayerSize,
                                  kPlayerSize, kPlayerSize);
  cinder::gl::draw(player_texture_, rectangle);
  cinder::gl::popModelMatrix();
}

void SpaceInvaders::AddInvader() {
  for (size_t j = 0; j < kInvaderRows; j++) {
    for (size_t i = 0; i < kInvaderCols; i++) {
      spaceinvaderslibrary::Invader invader = spaceinvaderslibrary::Invader
          (world_, i * 65 + 70, j * 75 + 150);

      if (j == kInvaderRows - 1) {
        front_invaders_.push_back(invader.GetBody());
      }

      invaders_.push_back(invader.GetBody());
    }
  }
}

void SpaceInvaders::AddShield() {
  for (size_t i = 0; i < kInvaderRows; i++) {
    spaceinvaderslibrary::Shield shield = spaceinvaderslibrary::Shield
        (world_, i * 200 + 100,getWindowHeight() - 120);

    shields_.push_back(shield.GetBody());
  }
}

void SpaceInvaders::AddShot() {
  if (front_invaders_.empty()) return;
  size_t random_invader = rand() % front_invaders_.size();

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(front_invaders_.at(random_invader)->GetPosition().x,
      front_invaders_.at(random_invader)->GetPosition().y + kInvaderSize);

  b2Body* body = world_->CreateBody(&bodyDef);

  b2CircleShape bullet;
  bullet.m_p.Set(kRadius, kRadius);
  bullet.m_radius = kRadius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &bullet;
  fixtureDef.density = 1.0f;

  body->CreateFixture(&fixtureDef);
  body->SetUserData((void*)"shot");
  invaders_shots_.push_back(body);
}

void SpaceInvaders::update() {
  if (state_ == GameState::kGameOver) {
    if (top_players_.empty()) {
      leaderboard_.
          AddScoreToLeaderBoard({player_name_, score_});
      top_players_ = leaderboard_.RetrieveHighScores(kLimit);
      player_scores_ = leaderboard_.
          RetrieveHighScores({player_name_, score_}, kLimit);

      // It is crucial the this vector be populated, given that `kLimit` > 0.
      assert(!top_players_.empty());
    }
    return;
  }

  for (size_t i = 0; i < 15; ++i) {
    world_->Step(1 / 30.0f, 10, 10);
    for (b2Contact* contact = world_->GetContactList(); contact;
         contact = contact->GetNext()) {
      if (contact->IsTouching()) {
        CheckMissileHitInvader(contact);
        CheckShieldDestroyed(contact);
        CheckInvaderShot(contact);
      }
    }

    const auto time = std::chrono::system_clock::now();
    double time_fire = std::chrono::duration_cast<std::chrono::milliseconds>(
        time - animation_time_elapsed_)
        .count();
    time_fire /= 1000.0;
    if (time_fire >= kWaitTime) {
      is_destroyed_ = false;
      animation_time_elapsed_ = time;
    }
  }

  //Restart background music if stopped playing.
  if (!background_voice_->isPlaying()) {
    background_voice_->start();
  }
}

void SpaceInvaders::RemoveBody(std::vector<b2Body*> &body_vector,
    b2Body* contact_body) {
  body_vector.erase(std::remove(body_vector.begin(),
                                body_vector.end(), contact_body),
                    body_vector.end());
}

void SpaceInvaders::CheckMissileHitInvader(b2Contact* contact) {
  if (contact->GetFixtureA()->GetBody()->GetUserData() == "invader" &&
      contact->GetFixtureB()->GetBody()->GetUserData() == "missile") {
    world_->DestroyBody(contact->GetFixtureA()->GetBody());
    world_->DestroyBody(contact->GetFixtureB()->GetBody());

    RemoveBody(missiles_, contact->GetFixtureB()->GetBody());
    RemoveBody(invaders_, contact->GetFixtureA()->GetBody());
    RemoveBody(front_invaders_, contact->GetFixtureA()->GetBody());

    score_ += 20;
    is_destroyed_ = true;
    animation_x_ = contact->GetFixtureA()->GetBody()->GetPosition().x;
    animation_y_ = contact->GetFixtureA()->GetBody()->GetPosition().y;

    cinder::audio::SourceFileRef invader_file =
        cinder::audio::load(
            cinder::app::loadAsset("invaderkilled.wav"));
    invader_killed_voice_ = cinder::audio::Voice::create(invader_file);
    // Start playing audio from the voice:
    invader_killed_voice_->start();
  }
}

void SpaceInvaders::CheckShieldDestroyed(b2Contact* contact) {
  if (contact->GetFixtureA()->GetBody()->GetUserData() ==
      "shield" &&
      contact->GetFixtureB()->GetBody()->GetUserData() ==
      "missile") {
    world_->DestroyBody(contact->GetFixtureA()->GetBody());
    world_->DestroyBody(contact->GetFixtureB()->GetBody());

    RemoveBody(missiles_, contact->GetFixtureB()->GetBody());
    RemoveBody(shields_ , contact->GetFixtureA()->GetBody());

  } else if (contact->GetFixtureA()->GetBody()->GetUserData() ==
             "shield" &&
             contact->GetFixtureB()->GetBody()->GetUserData() ==
             "shot") {
    world_->DestroyBody(contact->GetFixtureA()->GetBody());
    world_->DestroyBody(contact->GetFixtureB()->GetBody());

    RemoveBody(invaders_shots_, contact->GetFixtureB()->GetBody());
    RemoveBody(shields_ , contact->GetFixtureA()->GetBody());
  }
}

void SpaceInvaders::CheckInvaderShot(b2Contact* contact) {
  if (contact->GetFixtureA()->GetBody()->GetUserData() ==
      "player" &&
      contact->GetFixtureB()->GetBody()->GetUserData() ==
      "shot") {
    // Start playing audio from the voice:
    explosion_voice_->start();

    state_ = GameState::kGameOver;
  } else if (contact->GetFixtureA()->GetBody()->GetUserData() ==
             "shot" &&
             contact->GetFixtureB()->GetBody()->GetUserData() ==
             "missile") {
    world_->DestroyBody(contact->GetFixtureA()->GetBody());
    world_->DestroyBody(contact->GetFixtureB()->GetBody());

    RemoveBody(invaders_shots_, contact->GetFixtureA()->GetBody());
    RemoveBody(missiles_ , contact->GetFixtureB()->GetBody());

  } else if (contact->GetFixtureB()->GetBody()->GetUserData() ==
             "shot" &&
             contact->GetFixtureA()->GetBody()->GetUserData() ==
             "missile") {
    world_->DestroyBody(contact->GetFixtureA()->GetBody());
    world_->DestroyBody(contact->GetFixtureB()->GetBody());

    RemoveBody(invaders_shots_, contact->GetFixtureB()->GetBody());
    RemoveBody(missiles_ , contact->GetFixtureA()->GetBody());
  }
}

void SpaceInvaders::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  if (state_ == GameState::kStart) {
    AddShield();
    AddInvader();
    state_ = GameState::kPlaying;
  }

  if (state_ == GameState::kGameOver) {
    cinder::gl::clear(cinder::Color(1, 0, 0));
    DrawGameOver();
    return;
  }

  if (invaders_.empty() && state_ == GameState::kPlaying) {
    state_ = GameState::kGameOver;
  }

  AddPlayer();
  DrawScore();

  const auto time = std::chrono::system_clock::now();
  double time_shot = std::chrono::duration_cast<std::chrono::milliseconds>(
      time - shot_elapsed_)
      .count();
  time_shot /= 1000.0;
  
  if (time_shot >= kWaitTime) {
    AddShot();
    shot_elapsed_ = time;
  }

  cinder::gl::color(0, 1, 0);
  DrawInvaderShot();
  DrawMissile();
  DrawAnimation();
  DrawInvader();
  DrawShield();
}

void SpaceInvaders::DrawShield() {
  for (const auto& shield : shields_) {
    cinder::gl::pushModelMatrix();
    cinder::gl::translate(shield->GetPosition().x, shield->GetPosition().y);
    cinder::gl::rotate(shield->GetAngle());

    cinder::Rectf rectangle = cinder::Rectf(-45,-25,45,25);
    cinder::gl::draw(shield_texture_, rectangle);

    cinder::gl::popModelMatrix();
  }
}

void SpaceInvaders::DrawInvader() {
  for (const auto& invader : invaders_) {
    cinder::gl::pushModelMatrix();
    cinder::gl::translate(invader->GetPosition().x, invader ->GetPosition().y);
    cinder::gl::rotate(invader->GetAngle());

    cinder::Rectf rectangle = cinder::Rectf
        (-kInvaderSize, -kInvaderSize, kInvaderSize, kInvaderSize);
    cinder::gl::draw(invader_texture_, rectangle);

    cinder::gl::popModelMatrix();
  }
}

void SpaceInvaders::DrawMissile() {
  for(const auto& missiles : missiles_) {
    cinder::gl::pushModelMatrix();
    cinder::gl::translate(missiles->GetPosition().x, missiles->GetPosition().y);
    cinder::gl::rotate(missiles->GetAngle());

    cinder::gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);
    missiles->SetLinearVelocity(b2Vec2(0.0f, -kVelocity));

    cinder::gl::popModelMatrix();
  }
}

void SpaceInvaders::DrawInvaderShot() {
  if (!invaders_shots_.empty()) {
    cinder::gl::pushModelMatrix();
    cinder::gl::translate(invaders_shots_.back() ->GetPosition().x,
                  invaders_shots_.back()->GetPosition().y + kInvaderSize);
    cinder::gl::rotate(invaders_shots_.back()->GetAngle());

    cinder::gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);
    invaders_shots_.back()->SetLinearVelocity(b2Vec2(0.0f, kVelocity));

    cinder::gl::popModelMatrix();
  }
}

void SpaceInvaders::DrawAnimation() {
  if (is_destroyed_) {
    cinder::gl::pushModelMatrix();
    cinder::gl::translate(animation_x_, animation_y_);

    cinder::Rectf rectangle = cinder::Rectf(-kInvaderSize,-kInvaderSize,
                                    kInvaderSize, kInvaderSize);
    cinder::gl::draw(fire_texture_, rectangle);

    cinder::gl::popModelMatrix();
  }
}

void SpaceInvaders::DrawScore() {
  const cinder::vec2 center = getWindowCenter();
  const std::string text = "Current Score: " + std::to_string(score_);
  const cinder::Color color = cinder::Color::white();
  const cinder::ivec2 size = {300, 50};
  const cinder::vec2 loc = {center.x , 50};

  PrintText(text, color, size, loc);
}

void SpaceInvaders::DrawGameOver() {
  // Lazily print.
  if (top_players_.empty()) return;
  if (player_scores_.empty()) return;

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {500, 50};
  const cinder::Color color = cinder::Color::black();

  if (score_ == kMaxScore) {
    cinder::gl::clear(cinder::Color(0, 1, 0));
    PrintText("You WIN!!", color, size, center);
  } else {
    PrintText("Game Over :(", color, size, center);
  }

  PrintText("Press R to Replay!", color, size, {center.x, 50});

  const std::string your_score = "Your Score: " + std::to_string(score_);
  PrintText(your_score, color, size, {center.x, 200});
  size_t row = 0;

  for (const spaceinvaderslibrary::Player& player : top_players_) {
    std::stringstream ss;
    ss << player.name << " - " << player.score;
    PrintText(ss.str(), color, size, {center.x, center.y +(++row)*50});
  }
}

void SpaceInvaders::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_LEFT: {
      if (player_x_ - 2 * kPlayerSize >= 0) {
        player_x_ -= kPlayerSize;
      }
      break;
    }
    case KeyEvent::KEY_RIGHT: {
      if (player_x_ + 2 * kPlayerSize <= getWindowWidth()) {
        player_x_ += kPlayerSize;
      }
      break;
    }
    case KeyEvent::KEY_r: {
      ResetGame();
      break;
    }
    case KeyEvent::KEY_SPACE: {
      AddMissile({player_x_, player_y_});

      cinder::audio::SourceFileRef shoot_file =
          cinder::audio::load
              (cinder::app::loadAsset("shoot.wav"));
      fire_voice_ = cinder::audio::Voice::create(shoot_file);
      // Start playing audio from the voice:
      fire_voice_->start();
      break;
    }
    case KeyEvent::KEY_ESCAPE: {
      exit(0);
    }
  }
}

void SpaceInvaders::ResetGame() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  b2Vec2 gravity(0.0f, 0.0f);
  world_ = new b2World(gravity);
  score_ = 0;
  state_ = GameState::kStart;
  top_players_.clear();
  missiles_.clear();
  invaders_.clear();
  shields_.clear();
  front_invaders_.clear();
  invaders_shots_.clear();
  is_destroyed_ = false;
  animation_x_ = 0;
  animation_y_ = 0;
  player_y_ = getWindowHeight() - 30;
  player_x_ = getWindowWidth() / 2;
}

}  // namespace spaceinvaders


