// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <vector>
#include <irrlicht.h>

class EventReceiver : public irr::IEventReceiver
{
  irr::scene::ISceneNode *node;
  bool button_pressed;
  int  old_x, old_y;

  bool display_map;
  bool remove_map;

  bool display_menu;

  bool keyboard(const irr::SEvent &event);
  bool mouse(const irr::SEvent &event);

public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  void set_node(irr::scene::ISceneNode *node);

  bool is_mouse_pressed(int &x, int &y);

  bool get_display_map();
  bool get_remove_map();
  bool get_display_menu();
};

#endif

