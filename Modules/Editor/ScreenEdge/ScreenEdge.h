#ifndef SCREEN_EDGE_H
#define SCREEN_EDGE_H

#include <vector>
#include <SDL/SDL.h>

#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/MiscFunctions.h>

#include "DockedDialog.h"

class ScreenEdge {
  protected:
  const static float TAB_SIZE;
  const static float TAB_WIDTH;
  std::vector<DockedDialog*> cDockedDialogs;
  std::map<DockedDialog*, float> cCollapsingDialogs;
  DockedDialog* cExpandedDialog;
  float cAnimation;

  virtual float getTabX(DockedDialog*) = 0;
  virtual float getTabY(DockedDialog*) = 0;
  virtual float getTabWidth(DockedDialog*) = 0;
  virtual float getTabHeight(DockedDialog*) = 0;
  virtual void renderTab(DockedDialog*, float, float) = 0;

  bool mouseButtonDown(SDL_Event&);

  public:
  ScreenEdge();
    
  void update(int milliseconds);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
  void add(Dialog*);
};

#endif
