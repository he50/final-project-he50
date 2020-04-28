// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include "space_invaders.h"

using cinder::app::App;
using cinder::app::RendererGl;


namespace spaceinvaders {

const int kSamples = 8;
const int kWidth = 800;
const int kHeight = 800;

void SetUp(App::Settings* settings) {
  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("My CS 126 Application");
}

}  // namespace myapp


// This is a macro that runs the application.
CINDER_APP(spaceinvaders::MyApp,
           RendererGl(RendererGl::Options().msaa(spaceinvaders::kSamples)),
           spaceinvaders::SetUp)
