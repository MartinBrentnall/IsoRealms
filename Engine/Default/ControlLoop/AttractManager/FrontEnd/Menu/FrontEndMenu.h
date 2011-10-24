#ifndef FRONT_END_MENU_H
#define FRONT_END_MENU_H

#include <SDL/SDL.h>
#include <string>
#include <vector>

#include "EnterMenuCommand.h"
#include "FrontEndMenuItem.h"
#include "IFrontEndCommands.h"
#include "IMenuStack.h"

#include <IsoRealms/System.h>

class FrontEndMenu {
  private:
  std::string cMenuName;
  std::vector<FrontEndMenuItem*> cMenuItems;
  unsigned int cSelectedItem;
  
  void keyDown(SDLKey&);
  void mouseButtonDown();
  
  public:
  FrontEndMenu(IFrontEndCommands*, IMenuStack*, DOMNodeWrapper*);
  FrontEndMenu(IFrontEndCommands*, DOMNodeWrapper*);
  
  bool input(SDL_Event&);
  void render(float, IFont*);
};

#endif
