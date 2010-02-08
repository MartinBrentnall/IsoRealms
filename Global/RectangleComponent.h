/*
 * Copyright 2009 Martin Brentnall
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
#ifndef RECTANGLE_COMPONENT_H
#define RECTANGLE_COMPONENT_H

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "AbstractRectangularComponent.h"
#include "Configuration.h"
#include "IComponentContainer.h"
#include "IHUDComponent.h"
#include "IRectangularComponent.h"
#include "ScreenConfiguration.h"

// TODO: Rename Dialog
/**
 * When extending this class, be sure to call render() before doing your own
 * rendering, and input after processing your own input (if required).
 */
class RectangleComponent:public IRectangularComponent {
  private:
  static const float TITLE_BAR_HEIGHT;

  static IFont* cFont;

  std::string* cTitle;

  protected:
  float cX;
  float cY;
  float cWidth;
  float cHeight;
  bool cDragging;
  IComponentContainer* cComponentContainer;

  public:
  void static setFont(IFont*);

  RectangleComponent(IComponentContainer*, std::string*, float, float, float, float);

  void translate(float, float);
  void resize(float, float);

  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);

  /**
   * Close this component.
   */
  void close();  

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
};

#endif
