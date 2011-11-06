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
#ifndef SCREEN_CONFIGURATION_H
#define SCREEN_CONFIGURATION_H

#include "DOMNodeWrapper.h"
#include "InitException.h"
#include "ParseException.h"
#include "ScreenMode.h"
#include "Utils.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <iostream>
#include <string>

class ScreenConfiguration {
  private:
  SDL_Surface *cWindow;
  bool cFullScreen;
  int cScreenWidth;
  int cScreenHeight;
  int cScreenDepth;

  void parseSize(DOMNodeWrapper*);

  int getVideoFlags();
  
  void resizeScreen();
  
  public:

  int convertToXPixels(float);
  int convertToYPixels(float);

  ScreenConfiguration();
  
  /**
   * Construct the screen configuration from the specified node.
   */
  ScreenConfiguration(DOMNodeWrapper*);

  /**
   * Open a window on the screen according to the current configuration.
   * The window is given the specified title.
   */
  void openScreen(std::string);

  /**
   * Return the aspect ratio of the screen.
   */
  float getAspectRatio();

  /**
   * Return default GL X coordinate for specified screen pixel column.
   */
  float getXLocation(int);

  /**
   * Return default GL Y coordinate for specified screen pixel row.
   */
  float getYLocation(int);
  
  std::vector<ScreenMode*> getAvailableModes();
  
  void setMode(ScreenMode*);
};

#endif
