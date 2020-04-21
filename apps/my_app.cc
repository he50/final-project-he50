// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include "cinder/app/RendererGl.h"
#include <Box2D/Box2D.h>
#include <vector>
// EXAMPLE FROM:
// https://github.com/cinder/Cinder/blob/master/blocks/Box2D/
// templates/Basic%20Box2D/src/_TBOX_PREFIX_App.cpp


namespace myapp {

using cinder::app::KeyEvent;

using namespace ci;
using namespace ci::app;

b2World	*mWorld;
const float BOX_SIZE = 10;
std::vector<b2Body*> mBoxes;

MyApp::MyApp() {
}

void MyApp::setup() {
  b2Vec2 gravity( 0.0f, -10.0f );
  mWorld = new b2World( gravity );

  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set( 0.0f, getWindowHeight() );
  b2Body* groundBody = mWorld->CreateBody( &groundBodyDef );

  // Define the ground box shape.
  b2PolygonShape groundBox;

  // The extents are the half-widths of the box.
  groundBox.SetAsBox( getWindowWidth(), 10.0f );

  // Add the ground fixture to the ground body.
  groundBody->CreateFixture( &groundBox, 0.0f );
}

void MyApp::addBox(const vec2 &pos) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set( pos.x, pos.y );

  b2Body *body = mWorld->CreateBody( &bodyDef );

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox( BOX_SIZE, BOX_SIZE );

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.5f; // bounce

  body->CreateFixture( &fixtureDef );
  mBoxes.push_back( body );
}

void MyApp::update() {
  for( int i = 0; i < 10; ++i )
    mWorld->Step( 1 / 30.0f, 10, 10 );
}

void MyApp::draw() {
  gl::clear();

  gl::color( 1, 0.5f, 0.25f );
  for( const auto &box : mBoxes ) {
    gl::pushModelMatrix();
    gl::translate( box->GetPosition().x, box->GetPosition().y );
    gl::rotate( box->GetAngle() );

    gl::drawSolidRect( Rectf( -1, -10, 1, 10 ) );

    gl::popModelMatrix();
  }
}

void MyApp::mouseDown(MouseEvent event) {
  addBox(event.getPos());
}

void MyApp::keyDown(KeyEvent event) { }

}  // namespace myapp
