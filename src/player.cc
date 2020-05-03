//
// Created by Jonathan He on 4/28/20.
//
#include <string>
#include <Box2D/Box2D.h>
#include <spaceinvaderslibrary/player.h>

namespace spaceinvaderslibrary {

void Player::SetBody(b2World *world, int player_x, int player_y) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_kinematicBody;
  bodyDef.position.Set(player_x, player_y);

  player_body_ = world->CreateBody(&bodyDef);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(kPlayerSize, kPlayerSize);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;

  player_body_->CreateFixture(&fixtureDef);

  player_body_->SetUserData((void*)"player");
}

b2Body* Player::GetBody() {
  return player_body_;
}

}  // namespace spaceinvaderslibrary