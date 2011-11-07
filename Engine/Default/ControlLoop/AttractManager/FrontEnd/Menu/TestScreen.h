#ifndef TEST_SCREEN_H
#define TEST_SCREEN_H

#include <SDL/SDL.h>
#include <vector>

#include <IsoRealms/IFont.h>

#include "IApplicableItem.h"
#include "IFrontEndMenu.h"
#include "IMenuStack.h"

class TestScreen:public IFrontEndMenu {
  private:
  std::vector<IApplicableItem*> cApplicableItems;
  int cTimeOut;
  bool cConfirmed;
  IMenuStack* cMenuStack;

  bool keyDown(SDLKey&);
  
  public:
  TestScreen(std::vector<IApplicableItem*>, IMenuStack*);

  /****************************\
   * Implements IFrontEndMenu *
  \****************************/
  bool input(SDL_Event&);
  void render(float, IFont*);
  void update(unsigned int);
};

#endif
