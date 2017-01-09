// main.cpp

#include <irrlicht.h>

#include "events.h"
#include "scene.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

int main()
{
  // Le gestionnaire d'événements
  myScene maScene;
  maScene.scene_init();
  maScene.scene_run();

  return 0;
}
