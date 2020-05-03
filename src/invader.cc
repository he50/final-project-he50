//
// Created by Jonathan He on 4/28/20.
//

#include "spaceinvaderslibrary/invader.h"
#include <Box2D/Box2D.h>

namespace spaceinvaderslibrary {

Invader::Invader(b2World *world, int x_coordinate, int y_coordinate) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x_coordinate, y_coordinate);

  invader_body_ = world->CreateBody(&bodyDef);

  b2PolygonShape alien_shape;
  alien_shape.SetAsBox(18, 18);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &alien_shape;
  fixtureDef.density = 1.0f;

  invader_body_->CreateFixture(&fixtureDef);

  invader_body_->SetUserData((void*)"invader");
}

b2Body* Invader::GetBody() {
  return invader_body_;
}

}  // namespace spaceinvaderslibrary