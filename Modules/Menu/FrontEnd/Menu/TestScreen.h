#ifndef TEST_SCREEN_H
#define TEST_SCREEN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <sstream>
#include <vector>

#include <IsoRealms/Resources/Font/IFont.h>

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
  void cancelled();
};

#endif
