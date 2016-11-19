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
#include "ITextComponent.h"
#include "ListBox.h"
#include "LookAndFeel.h"
#include "RectangularComponent.h"
#include "ScrollableContainer.h"
#include "TabbedContainer.h"
#include "TextLabelComponent.h"
#include "TextFieldComponent.h"

class Dialog : public RectangularComponent {
  private:
  static const float TITLE_BAR_HEIGHT;

  std::vector<IComponentCloseListener*> cCloseListeners;
//  RectangularComponent* cLoadedComponent;
  std::string cTitle;

  protected:
  float cX;
  float cY;
  float cWidth;
  float cHeight;
  bool cDragging;
  IComponentContainer* cComponentContainer;
  
  // Properties
  bool cMovable;
  bool cClosable;

  public:
  Dialog(IComponentContainer*, const std::string&, IResourceAccessor*);
  Dialog(IComponentContainer*, const std::string&, float, float, float, float);

  void setTitle(const std::string&);
  
  void translate(float, float);
  void resize(float, float);
  void setSize(float, float, float, float);
  void setMovable(bool);
  void setClosable(bool);

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

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);
  virtual bool contains(float, float);

  virtual void renderDialogContent() {};
  virtual void updateDialogContent(int) {};
  virtual bool inputDialogContent(SDL_Event&) {return false;};
  
  /***************************************************\
   * Implements IRectangle (in IRectangularComponent *
  \***************************************************/
  float getLeft();
  float getBottom();
  float getRight();
  float getTop();
  
  float getWidth();
  float getHeight();

  virtual ~Dialog();
};

#endif
