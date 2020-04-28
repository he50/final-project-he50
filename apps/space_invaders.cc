// Copyright (c) 2020 [Your Name]. All rights reserved.
#include "space_invaders.h"

#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <mylibrary/invader.h>
#include <mylibrary/player.h>
#include <mylibrary/shield.h>

#include <vector>

#include "cinder/app/RendererGl.h"
#include "cinder/audio/audio.h"

const char kDbPath[] = "snake.db";
const char kNormalFont[] = "Arial";
using cinder::audio::VoiceRef;

// EXAMPLE FROM:
// https://github.com/cinder/Cinder/blob/master/blocks/Box2D/
// templates/Basic%20Box2D/src/_TBOX_PREFIX_App.cpp


namespace spaceinvaders {

using cinder::app::KeyEvent;

using namespace ci;
using namespace ci::app;

MyApp::MyApp()
    : leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      player_y_{getWindowHeight() - 30},
      player_x_{getWindowWidth() / 2},
      kBoxSize{20},
      kRadius{5}{}

void MyApp::setup() {
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
}

void MyApp::AddMissile(const vec2 &pos) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos.x, pos.y - 20);

  b2Body *body = world_->CreateBody(&bodyDef);

//  b2PolygonShape dynamicBox;
//  dynamicBox.SetAsBox(5.0f, 5.0f);
//
//  b2FixtureDef fixtureDef;
//  fixtureDef.shape = &dynamicBox;
//  fixtureDef.density = 1.0f;

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

void MyApp::AddPlayer() {
  mylibrary::Player player = mylibrary::Player();
  player.SetBody(world_, player_x_, player_y_);

  gl::pushModelMatrix();
  gl::translate(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y);
  gl::rotate(player.GetBody()->GetAngle());

  cinder::Rectf rectangle = Rectf( -kBoxSize, -kBoxSize, kBoxSize, kBoxSize );
  cinder::gl::draw(player_texture_, rectangle);

  gl::popModelMatrix();
}

void MyApp::AddInvader() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 11; i++) {
      mylibrary::Invader invader = mylibrary::Invader
          (world_, i * 65 + 70, j * 75 + 150);

      invaders_.push_back(invader.GetBody());
    }
  }
}

void MyApp::AddShield() {
  for (int i = 0; i < 4; i++) {

    mylibrary::Shield shield = mylibrary::Shield
        (world_, i * 200 + 100, getWindowHeight() - 120);

    shields_.push_back(shield.GetBody());
  }
}

void MyApp::update() {
  for (int i = 0; i < 10; ++i) {
    world_->Step(1 / 30.0f, 10, 10);
    for (b2Contact* contact = world_->GetContactList(); contact; contact = contact->GetNext()) {

      if (contact->IsTouching()) {

        if (contact->GetFixtureA()->GetBody()->GetUserData() == "invader" && contact->GetFixtureB()->GetBody()->GetUserData() == "missile") {
          world_->DestroyBody(contact->GetFixtureA()->GetBody());
          world_->DestroyBody(contact->GetFixtureB()->GetBody());

          missiles_.erase(std::remove(missiles_.begin(), missiles_.end(),
                                      contact->GetFixtureB()->GetBody()),
                          missiles_.end());

          invaders_.erase(std::remove(invaders_.begin(), invaders_.end(),
                                    contact->GetFixtureA()->GetBody()),
                        invaders_.end());

          score_ += 20;
          is_destroyed_ = true;
          animation_x_ = contact->GetFixtureA()->GetBody()->GetPosition().x;
          animation_y_ = contact->GetFixtureA()->GetBody()->GetPosition().y;

          cinder::audio::SourceFileRef sourceFile =
              cinder::audio::load(cinder::app::loadAsset("invaderkilled.wav"));
          invader_killed_voice_ = cinder::audio::Voice::create(sourceFile);

          // Start playing audio from the voice:
          invader_killed_voice_->start();
        }
      }
    }

    const auto time = std::chrono::system_clock::now();

    double time_fire = std::chrono::duration_cast<std::chrono::milliseconds>
        (time - animation_time_elapsed_).count();
    time_fire /= 1000.0;

    if (time_fire >= 1.0) {
      is_destroyed_ = false;
      animation_time_elapsed_ = time;
    }
  }
}


void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  gl::clear();

  AddPlayer();
  AddShield();

  if (invaders_.empty()) {
    AddInvader();
  }

  gl::color( 0, 1, 0 );
  for(const auto& missiles : missiles_) {
//    gl::pushModelMatrix();
//    gl::translate(missiles->GetPosition().x, missiles->GetPosition().y);
//    gl::rotate(missiles->GetAngle());
//
//    gl::drawSolidRect(Rectf( -1, -5, 1, 5 ));
//    missiles->SetLinearVelocity({0.0f, -30.0f});
//
//    gl::popModelMatrix();


    gl::pushModelMatrix();
    gl::translate(missiles->GetPosition().x, missiles->GetPosition().y);
    gl::rotate(missiles->GetAngle());
    gl::drawSolidCircle(cinder::vec2(0, 0), kRadius);
    missiles->SetLinearVelocity(b2Vec2(0.0f, -30.0f));
    gl::popModelMatrix();
  }

  for (const auto& invader : invaders_) {
    gl::pushModelMatrix();

    gl::translate(invader->GetPosition().x, invader ->GetPosition().y);
    gl::rotate(invader->GetAngle());

    cinder::Rectf rectangle = Rectf(-18, -18, 18, 18);
    cinder::gl::draw(invader_texture_, rectangle);

    gl::popModelMatrix();
  }

  for (const auto& shield : shields_) {
    gl::pushModelMatrix();
    gl::translate(shield->GetPosition().x, shield->GetPosition().y);
    gl::rotate(shield->GetAngle());

    cinder::Rectf rectangle = Rectf(-45, -25, 45, 25);
    cinder::gl::draw(shield_texture_, rectangle);

    gl::popModelMatrix();
  }

}


void MyApp::mouseDown(MouseEvent event) {
  //addMissiles(event.getPos());
}

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_LEFT: {
      if (player_x_ - 40 >= 0) {
        player_x_ -= 20;
      }
      break;
    }
    case KeyEvent::KEY_RIGHT: {
      if (player_x_ + 40 <= getWindowWidth()) {
        player_x_ += 20;
      }
      break;
    }
    case KeyEvent::KEY_SPACE: {
      AddMissile({player_x_, player_y_});

      cinder::audio::SourceFileRef sourceFile =
          cinder::audio::load
              (cinder::app::loadAsset("shoot.wav"));
      fire_voice_ = cinder::audio::Voice::create( sourceFile );

      // Start playing audio from the voice:
      fire_voice_->start();
      break;
    }
    case KeyEvent::KEY_ESCAPE: {
      exit(0);
    }
  }
}

}  // namespace spaceinvaders

