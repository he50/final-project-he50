// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <gflags/gflags.h>

#include <string>
#include <vector>

#include "space_invaders.h"

using cinder::app::App;
using cinder::app::RendererGl;
using std::string;
using std::vector;
DEFINE_string(name, "CS126SP20", "the name of the player");

namespace spaceinvaders {

void ParseArgs(vector<std::string>* args) {
  gflags::SetUsageMessage(
      "Play a game of Snake. Pass --helpshort for options.");
  int argc = static_cast<int>(args->size());

  vector<char*> argvs;
  for (string& str : *args) {
    argvs.push_back(&str[0]);
  }

  char** argv = argvs.data();
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

const int kSamples = 8;
const int kWidth = 800;
const int kHeight = 800;

void SetUp(App::Settings* settings) {
  vector<string> args = settings->getCommandLineArgs();
  ParseArgs(&args);

  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("Space Invaders");
}

}  // namespace myapp


// This is a macro that runs the application.
CINDER_APP(spaceinvaders::SpaceInvaders,
           RendererGl(RendererGl::Options().msaa(spaceinvaders::kSamples)),
           spaceinvaders::SetUp)
