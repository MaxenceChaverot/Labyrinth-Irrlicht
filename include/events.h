// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <vector>
#include <irrlicht.h>

class EventReceiver : public irr::IEventReceiver
{
  irr::scene::ISceneNode *node;

  // Mouse event
  bool button_pressed;
  int  old_x, old_y;

  // Booleans to display the map and the pause menu
  bool display_map;
  bool remove_map;

  // Event functions
  bool keyboard(const irr::SEvent &event);
  bool mouse(const irr::SEvent &event);

public:

  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  void set_node(irr::scene::ISceneNode *node);

  // Get the click coordinates
  bool is_mouse_pressed(int &x, int &y);

  // Access booleans
  bool get_display_map();
  bool get_remove_map();
};

#endif

