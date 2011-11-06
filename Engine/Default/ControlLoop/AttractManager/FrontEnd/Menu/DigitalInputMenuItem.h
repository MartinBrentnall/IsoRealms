#ifndef DIGITAL_INPUT_MENU_ITEM_H
#define DIGITAL_INPUT_MENU_ITEM_H

#include <string>

#include <IsoRealms/DigitalInput.h>

#include "IFrontEndMenuItem.h"

class DigitalInputMenuItem:public IFrontEndMenuItem {
  private:
  std::string cName;
  DigitalInput* cDigitalInput;

  public:
  DigitalInputMenuItem(const std::string&, DigitalInput*);
      
  /********************************\
   * Implements IFrontEndMenuItem *
  \********************************/
  void render(int, float, IFont*, bool);
  FocusAction keyDown(SDLKey&);
};

#endif
