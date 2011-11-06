#ifndef FRONT_END_MENU_H
#define FRONT_END_MENU_H

#include <SDL/SDL.h>
#include <string>
#include <vector>

#include <IsoRealms/InputCommands.h>
#include <IsoRealms/System.h>

#include "DigitalInputMenuItem.h"
#include "EnterMenuCommand.h"
#include "FocusAction.h"
#include "FrontEndMenuItem.h"
#include "IFrontEndCommands.h"
#include "IFrontEndMenuItem.h"
#include "IMenuStack.h"

class FrontEndMenu {
  private:
  std::string cMenuName;
  std::vector<IFrontEndMenuItem*> cMenuItems;
  unsigned int cSelectedItem;
  
  void keyDown(SDLKey&);
  
  std::vector<IFrontEndMenuItem*> parseControlConfig(const std::string&);
  
  bool isNumeric(const std::string&, std::size_t, std::size_t);
  std::size_t getFormatChars(const std::string&, std::size_t);
  std::string format(const std::string&, std::vector<std::string>);
  
  
  public:
  FrontEndMenu(IFrontEndCommands*, IMenuStack*, DOMNodeWrapper*, const std::string&, std::vector<std::string>);
  FrontEndMenu(IFrontEndCommands*, DOMNodeWrapper*);
  
  bool input(SDL_Event&);
  void render(float, IFont*);
};

#endif
