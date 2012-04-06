/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef DIALOG_H
#define DIALOG_H

#include <GL/glew.h>
#include <map>
#include <SDL/SDL.h>
#include <vector>

#include "../Configuration.h"
#include "../IComponentContainer.h"
#include "../IHUDComponent.h"
#include "../ScreenConfiguration.h"
#include "../Utils.h"

#include "AbstractRectangularComponent.h"
#include "Button.h"
#include "ComponentEdgeLayout.h"
#include "EdgeRelation.h"
#include "FlexibleGridLayoutComponent.h"
#include "GridLayoutComponent.h"
#include "IComponentBoundsCalculator.h"
#include "IComponentCloseListener.h"
#include "IRectangularComponent.h"
#include "ITextComponent.h"
#include "ListBox.h"
#include "LookAndFeel.h"
#include "ScrollableContainer.h"
#include "TextLabelComponent.h"
#include "TextFieldComponent.h"

class Dialog:public IRectangularComponent {
  private:
  static const float TITLE_BAR_HEIGHT;

  std::map<std::string, ISizedComponent*> cSizedComponents;
  std::map<std::string, Button*> cCommandableComponents;
  std::map<std::string, ListBox*> cListBoxComponents;
  // TODO: Should use interface to support other components; e.g. ListBoxes
  std::map<std::string, TextFieldComponent*> cStringValueComponents;
  std::string cTitle;
  std::vector<IComponentCloseListener*> cCloseListeners;
  std::vector<IHUDComponent*> cChildren;
  IHUDComponent* cFocusedComponent;

  void loadDialog(DOMNodeWrapper*, IRectangle*, float);
  void setComponentText(DOMNodeWrapper*, ITextComponent*);
  IComponentBoundsCalculator* getBoundsCalculator(DOMNodeWrapper*, IRectangle*, float, ISizedComponent*);
  ISizedComponent* loadSizedComponent(DOMNodeWrapper*);
  void loadFlexibleGridCells(DOMNodeWrapper*, FlexibleGridLayoutComponent*);
  void loadEvenGridCells(DOMNodeWrapper*, GridLayoutComponent*);

  void testFocusChange(SDL_Event& event);

  protected:
  float cX;
  float cY;
  float cWidth;
  float cHeight;
  bool cDragging;
  IComponentContainer* cComponentContainer;

  public:
  Dialog(IComponentContainer*, const std::string&);
  Dialog(IComponentContainer*, const std::string&, float, float, float, float);

  void translate(float, float);
  void resize(float, float);

  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);

  /**
   * Call this function to tell the dialog that we probably wish to do
   * something outside the dialog.  
   */
  void relinquishFocus();

  /**
   * Close this component.
   */
  void close();  

  void addCloseListener(IComponentCloseListener*);

  void addComponent(IHUDComponent*);
  void setFocusedComponent(IHUDComponent*);
  void setComponentAction(const std::string&, ICommand*);
  ListBox* getListBox(const std::string&);
  std::string getStringValue(const std::string&);

  virtual void renderContent() = 0;
  virtual void updateContent(int) = 0;
  virtual bool inputContent(SDL_Event&) = 0;

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
  virtual bool contains(float, float);

  /***************************************************\
   * Implements IRectangle (in IRectangularComponent *
  \***************************************************/
  float getLeft();
  float getBottom();
  float getRight();
  float getTop();

  virtual ~Dialog();
};

#endif
