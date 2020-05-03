//
// Created by Jonathan He on 4/28/20.
//

#include "spaceinvaderslibrary/shield.h"
#include <Box2D/Box2D.h>

namespace spaceinvaderslibrary {

Shield::Shield(b2World *world, int x_coordinate, int y_coordinate) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_kinematicBody;
  bodyDef.position.Set(x_coordinate, y_coordinate);

  shield_body_ = world->CreateBody(&bodyDef);

  b2PolygonShape alien_shape;
  alien_shape.SetAsBox(45, 25);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &alien_shape;
  fixtureDef.density = 1.0f;

  shield_body_->CreateFixture(&fixtureDef);

  shield_body_->SetUserData((void*)"shield");

}

b2Body* Shield::GetBody() {
  return shield_body_;
}

}  // namespace spaceinvaderslibrary
