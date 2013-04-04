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
#ifndef COLOUR_H
#define COLOUR_H

#include <GL/glew.h>
#include <iostream>

#include "../../Persistence/DOMNodeWrapper.h"
#include "../../Persistence/DOMNodeWriter.h"
#include "../../MiscFunctions.h"
#include "../../Utils.h"

#include "IColour.h"

class Colour:public IColour {
  private:

  /** Red intensity. */
  float cRed;

  /** Green intensity. */
  float cGreen;

  /** Blue intensity. */
  float cBlue;

  /** Alpha intensity. */
  float cAlpha;  

  public:
  /**
   * Construct opaque black.
   */
  Colour();
  
  Colour(const IColour&);
  
  /**
   * Construct an opaque colour of the specified intensity values, from 0 to 1.
   *
   * @param float  Red intensity.
   * @param float  Green intensity.
   * @param float  Blue intensity.
   */
  Colour(float, float, float);

  /**
   * Construct an colour of the specified intensity values, from 0 to 1.
   *
   * @param float  Red intensity.
   * @param float  Green intensity.
   * @param float  Blue intensity.
   * @param float  Alpha intensity.
   */
  Colour(float, float, float, float);

  /**
   * Construct a colour resulting from equal mixtures of the two specified colours.
   *
   * @param Colour  First colour.
   * @param Colour  Second colour.
   */
  Colour(IColour*, IColour*);

  /**
   * Construct a colour resulting from equal mixtures of the two specified colours.
   *
   * @param Colour  First colour.
   * @param Colour  Second colour.
   */
  Colour(IColour*, IColour*, float);

  /**
   * Create a colour of the specified relative intensity to the specified colour.
   * For example, 0.5 returns a colour of half the brightness of this, and
   * 2.0 returns a colour of double the brightness of this.
   *
   * @param float  The intensity of the new Colour.
   */
  Colour(IColour*, float);

  /**************************************************************************\
   * Colour modifiers                                                       *
  \**************************************************************************/
  /**
   * Set all intensity channels to the specified values.
   *
   * @param float  Red intensity.
   * @param float  Green intensity.
   * @param float  Blue intensity.
   * @param float  Alpha intensity.
   */
  void set(float, float, float, float);

  /**
   * Increment all channels of this colour to the specified respective
   * intensities.  Positive values increase intensity (1 being brightest)
   * and negative values decrease intensity (0 being darkest).  A value
   * of zero for a given channel will not change that channel.
   *
   * @param float  Red intensity.
   * @param float  Green intensity.
   * @param float  Blue intensity.
   * @param float  Alpha intensity.
   */
  void change(float, float, float, float);

  /**
   * Sets this colour as the GL rendering colour.
   */
  void set();

  /**
   * Set this colour to the background colour (glClearColor).
   */
  void setBackground();

  void save(DOMNodeWriter*, IResourceLocator*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /**********************\
   * Implements IColour *
  \**********************/
  float getRed() const;
  float getGreen() const;
  float getBlue() const;
  float getAlpha() const;
  void setRed(float);
  void setGreen(float);
  void setBlue(float);
  void setAlpha(float);
  int getIntRed() const;
  int getIntGreen() const;
  int getIntBlue() const;
  int getIntAlpha() const;
  float luminance();
  
  void debug();
};

#endif
