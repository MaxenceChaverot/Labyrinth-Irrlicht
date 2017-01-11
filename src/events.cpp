// events.cpp

#include <irrlicht.h>
#include <iostream>

#include "events.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
  : node(nullptr), button_pressed(false), display_map(false)
{
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
  if (event.KeyInput.PressedDown)
  {
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();
    switch (event.KeyInput.Key)
    {
       case KEY_KEY_M:
        display_map = !display_map; //the map will be displayed/closed when the user click on the key M
        break;
       case KEY_ESCAPE:
        exit(0);
       default:;
    }
    node->setPosition(position);
    node->setRotation(rotation);
  }
  return false;
}


/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse(const SEvent &event)
{
    switch(event.MouseInput.Event)
        {
        case EMIE_LMOUSE_PRESSED_DOWN:
            button_pressed = true;
            old_x = event.MouseInput.X;
            old_y = event.MouseInput.Y;
            break;
        case EMIE_LMOUSE_LEFT_UP:
            button_pressed = false;
            break;
        case EMIE_MOUSE_MOVED:
            if (button_pressed)
            {
                old_x = event.MouseInput.X;
                old_y = event.MouseInput.Y;
            }
            break;
        default:
            ;
        }

    return false;
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  if (!node) return false;
  switch (event.EventType)
  {
    case EET_KEY_INPUT_EVENT:
      return keyboard(event);
    case EET_MOUSE_INPUT_EVENT:
      return mouse(event);
    default:;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/
void EventReceiver::set_node(irr::scene::ISceneNode *n)
{
  node = n;
}

/**************************************************************************\
 * EventReceiver::is_mouse_pressed                                        *
\**************************************************************************/
bool EventReceiver::is_mouse_pressed(int &x, int &y)
{
    if(button_pressed)
    {
        x = old_x;
        y = old_y;
        return true;
    }
    return false;
}

/**************************************************************************\
 * EventReceiver::get_display_map                                       *
\**************************************************************************/
bool EventReceiver::get_display_map()
{
    return display_map;
}

/**************************************************************************\
 * EventReceiver::get_remove_map                                       *
\**************************************************************************/
bool EventReceiver::get_remove_map()
{
    return remove_map;
}
