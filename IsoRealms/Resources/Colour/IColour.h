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
#ifndef I_COLOUR_H
#define I_COLOUR_H

#include <string>

#include <IsoRealms/Resources/IResourceManageable.h>

class IColour:public IResourceManageable<IColour> {
  public:
  
  /**
   * Returns the cRed intensity of this colour.
   *
   * @returns  The cRed intensity of this colour.
   */
  virtual float getRed() const = 0;

  /**
   * Returns the green intensity of this colour.
   *
   * @returns  The green intensity of this colour.
   */
  virtual float getGreen() const = 0;

  /**
   * Returns the blue intensity of this colour.
   *
   * @returns  The blue intensity of this colour.
   */
  virtual float getBlue() const = 0;

  /**
   * Returns the alpha intensity of this colour.
   *
   * @returns  The alpha intensity of this colour.
   */
  virtual float getAlpha() const = 0;
  
  virtual int getIntRed() const = 0;
  virtual int getIntGreen() const = 0;
  virtual int getIntBlue() const = 0;
  virtual int getIntAlpha() const = 0;
  virtual float luminance() = 0;
  virtual void set() = 0;
  virtual void setBackground() = 0;

  virtual ~IColour() {}
};

#endif
