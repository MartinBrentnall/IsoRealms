#ifndef FRONT_END_MENU_ITEM_H
#define FRONT_END_MENU_ITEM_H

#include <GL/gl.h>
#include <string>

#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IFont.h>

#include "FocusAction.h"
#include "IFrontEndCommands.h"
#include "IFrontEndMenuItem.h"

class FrontEndMenuItem:public IFrontEndMenuItem {
  private:
  std::string cMenuText;
  ICommand* cCommand;

  public:
  FrontEndMenuItem(IFrontEndCommands*, DOMNodeWrapper*);
  FrontEndMenuItem(const std::string&, ICommand*);

  /********************************\
   * Implements IFrontEndMenuItem *
  \********************************/
  void render(int, float, IFont*, bool);
  FocusAction keyDown(SDLKey&);
};

#endif
