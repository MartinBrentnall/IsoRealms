#ifndef FRONT_END_MENU_ITEM_H
#define FRONT_END_MENU_ITEM_H

#include <GL/gl.h>
#include <string>

#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IFont.h>

#include "IFrontEndCommands.h"

class FrontEndMenuItem {
  private:
  std::string cMenuText;
  ICommand* cCommand;

  public:
  FrontEndMenuItem(IFrontEndCommands*, DOMNodeWrapper*);
  FrontEndMenuItem(const std::string&, ICommand*);
    
  void render(int, float, IFont*, bool);
  void execute();
};

#endif
