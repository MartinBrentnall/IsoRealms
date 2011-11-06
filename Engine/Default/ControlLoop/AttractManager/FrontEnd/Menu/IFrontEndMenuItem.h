#ifndef I_FRONT_END_MENU_ITEM_H
#define I_FRONT_END_MENU_ITEM_H

#include <SDL/SDL.h>

#include <IsoRealms/IFont.h>

#include "FocusAction.h"

class IFrontEndMenuItem {
  public:
  virtual void render(int, float, IFont*, bool) = 0;
  virtual FocusAction keyDown(SDLKey&) = 0;
};

#endif
