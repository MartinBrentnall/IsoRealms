/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
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
  DockedDialog* cDragging;
  bool cDragged = true;

  class TabIconLayout:public IComponentBoundsCalculator {
    private:
    ScreenEdge* cParent;
    DockedDialog* cDialog;
    
    public:
    TabIconLayout(ScreenEdge*, DockedDialog*);
    
    float getTop();
    float getBottom();
    float getLeft();
    float getRight();
  };  

  virtual float getTabX(DockedDialog*) = 0;
  virtual float getTabY(DockedDialog*) = 0;
  virtual float getTabWidth(DockedDialog*) = 0;
  virtual float getTabHeight(DockedDialog*) = 0;
  virtual void renderTab(DockedDialog*, float, float) = 0;
  virtual void moveTab(DockedDialog*, float, float) = 0;

  bool mouseButtonDown(SDL_Event&);
  bool mouseButtonUp(SDL_Event&);
  bool mouseMotion(SDL_Event&);

  public:
  ScreenEdge();
    
  void update(int milliseconds);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
  void add(Dialog*, AbstractRectangularComponent*, float);
};

#endif
