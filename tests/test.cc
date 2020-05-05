// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>

#include <spaceinvaderslibrary/player.h>
#include <spaceinvaderslibrary/shield.h>
#include <spaceinvaderslibrary/invader.h>



TEST_CASE("Player b2Body check", "[player]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  spaceinvaderslibrary::Player test_player = spaceinvaderslibrary::Player();
  test_player.SetBody(world, 10, 10);

  REQUIRE(test_player.GetBody()->GetPosition().x == 10);
  REQUIRE(test_player.GetBody()->GetPosition().y == 10);
  REQUIRE(test_player.GetBody()->GetUserData() == "player");
  REQUIRE(test_player.GetBody()->GetType() == b2_kinematicBody);

}

TEST_CASE("Shield b2Body check", "[shield]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  spaceinvaderslibrary::Shield test_shield = spaceinvaderslibrary::Shield
      (world, 50, 50);

  REQUIRE(test_shield.GetBody()->GetPosition().x == 50);
  REQUIRE(test_shield.GetBody()->GetPosition().y == 50);
  REQUIRE(test_shield.GetBody()->GetUserData() == "shield");
  REQUIRE(test_shield.GetBody()->GetType() == b2_kinematicBody);

}

TEST_CASE("Invader b2Body check", "[invader]") {
  b2Vec2 gravity(0.0f, 0.0f);
  b2World* world = new b2World(gravity);
  spaceinvaderslibrary::Invader test_invader = spaceinvaderslibrary::Invader
      (world, 20, 20);

  REQUIRE(test_invader.GetBody()->GetPosition().x == 20);
  REQUIRE(test_invader.GetBody()->GetPosition().y == 20);
  REQUIRE(test_invader.GetBody()->GetUserData() == "invader");
  REQUIRE(test_invader.GetBody()->GetType() == b2_dynamicBody);

}



