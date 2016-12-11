/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef I_FONT_H
#define I_FONT_H

#include <IsoRealms/Resources/IResourceManageable.h>

class IFont:public IResourceManageable<IFont> {
  public:
  enum Alignment {
    LEFT,
    CENTER,
    RIGHT
  };

  /**
   * This function will print out text at window coordinates x,y, using the font ft_font.
   * The current modelview matrix will also be applied to the text.
   * 
   * @param float  
   * @param float  
   * @param float  
   * @param int  
   * @param char*  
   * @param ...  
   */
  virtual void print(float, float, float, Alignment, const char*, ...) = 0;

  /**
   * This function calculates the width of the specified string when printed
   * using this font at the specified size.
   * 
   * @param float  
   * @param char*  
   * @param ...  
   * @returns The width of the text.
   */
  virtual float getWidth(float, const char*, ...) = 0;
  
  /**
   * This function calculates the height of the specified string when printed
   * using this font at the specified size.
   * 
   * @param float  
   * @param char*  
   * @param ...  
   * @returns The height of the text.
   */
  virtual float getHeight(float, const char*, ...) = 0;
  
  /**
   * Calculates where the caret should be based on the clicked position.
   * 
   * @param float
   * @param float
   * @param char*
   * @param ...
   * @returns Caret position.
   */
  virtual unsigned int getChar(float, float, const char*, ...) = 0;
  
  virtual ~IFont() {}
};

#endif
