// Copyright (c) 2020 [Dhruv Agrawal]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <gflags/gflags.h>

#include "space_impact_app.h"

using cinder::app::App;
using cinder::app::RendererGl;

namespace spaceimpactapp {

DEFINE_string(name, "CS126SP20", "the name of the player");

const int kSamples = 8;
const int kWidth = 800;
const int kHeight = 800;

void ParseArgs(std::vector<std::string>* args) {
  gflags::SetUsageMessage(
      "Play a game of Space Impact. Pass --helpshort for options.");
  int argc = static_cast<int>(args->size());

  std::vector<char*> argvs;
  for (std::string& str : *args) {
    argvs.push_back(&str[0]);
  }

  char** argv = argvs.data();
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

void SetUp(App::Settings* settings) {
  std::vector<std::string> args = settings->getCommandLineArgs();
  ParseArgs(&args);

  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("My CS 126 Application");
}

}  // namespace spaceimpactapp

// This is a macro that runs the application.
CINDER_APP(spaceimpactapp::SpaceImpactApp,
           RendererGl(RendererGl::Options().msaa(spaceimpactapp::kSamples)),
           spaceimpactapp::SetUp)
