// Copyright (c) 2020 [Your Name]. All rights reserved.
#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include "cinder/app/RendererGl.h"
#include "cinder/audio/audio.h"
#include <Box2D/Box2D.h>
#include <vector>

#include <mylibrary/invader.h>
#include <mylibrary/player.h>
#include <mylibrary/shield.h>

const char kDbPath[] = "snake.db";
const char kNormalFont[] = "Arial";
using cinder::audio::VoiceRef;

// EXAMPLE FROM:
// https://github.com/cinder/Cinder/blob/master/blocks/Box2D/
// templates/Basic%20Box2D/src/_TBOX_PREFIX_App.cpp


namespace myapp {

using cinder::app::KeyEvent;

using namespace ci;
using namespace ci::app;

MyApp::MyApp()
    : leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
      player_y_{getWindowHeight() - 30},
      player_x_{getWindowWidth() / 2},
      kRadius{5}{}

void MyApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  b2Vec2 gravity(0.0f, 0.0f);
  world_ = new b2World(gravity);

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
  bullet.m_p.Set(5.0f, 5.0f);
  bullet.m_radius = kRadius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &bullet;
  fixtureDef.density = 1.0f;

  body->CreateFixture(&fixtureDef);
  body->SetUserData((void*)"missile");
  missiles_.push_back(body);
}


void MyApp::update() {
  for( int i = 0; i < 10; ++i )
    world_->Step( 1 / 30.0f, 10, 10 );
}

void MyApp::draw() {
  gl::clear();

  gl::color( 1, 0.5f, 0.25f );
  for( const auto &missile : missiles_ ) {
    gl::pushModelMatrix();
    gl::translate( missile->GetPosition().x, missile->GetPosition().y );
    gl::rotate( missile->GetAngle() );

    gl::drawSolidRect( Rectf( -1, -10, 1, 10 ) );

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
      addMissile({player_x_, player_y_});

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

}  // namespace myapp

