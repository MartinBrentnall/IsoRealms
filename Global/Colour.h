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

#include <GL/gl.h>
#include <iostream>

#include "DOMNodeWrapper.h"
#include "DOMNodeWriter.h"
#include "IllegalStateException.h"
#include "MiscFunctions.h"
#include "Utils.h"

class Colour {
  public:

  /** Red intensity. */
  float cRed;

  /** Green intensity. */
  float cGreen;

  /** Blue intensity. */
  float cBlue;

  /** Alpha intensity. */
  float cAlpha;  

  public:
  enum Channel {
    RED,
    GREEN,
    BLUE,
    ALPHA
  };

  /**************************************************************************\
   * Constructors                                                           *
  \**************************************************************************/
  /**
   * Construct opaque black.
   */
  Colour();

  Colour(DOMNodeWrapper*);

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
  Colour(Colour, Colour);

  /**
   * Returns a colour of the specified relative intensity to this colour.
   * For example, 0.5 returns a colour of half the brightness of this, and
   * 2.0 returns a colour of double the brightness of this.
   *
   * @param float  The intensity of the new Colour.
   */
  Colour brightness(float);

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

  /**************************************************************************\
   * OpenGL helper functions                                                *
  \**************************************************************************/
  /**
   * Sets this colour as the GL rendering colour.
   */
  void set();

  /**
   * Set this colour to the background colour (glClearColor).
   */
  void setBackground();

  /**************************************************************************\
   * Colour channel access                                                  *
  \**************************************************************************/
  /**
   * TODO: Replace with [] operator?
   * Returns the cRed intensity of this colour.
   *
   * @returns  The cRed intensity of this colour.
   */
  float getRed();

  /**
   * Returns the green intensity of this colour.
   *
   * @returns  The green intensity of this colour.
   */
  float getGreen();

  /**
   * Returns the blue intensity of this colour.
   *
   * @returns  The blue intensity of this colour.
   */
  float getBlue();

  /**
   * Get the colour of the specified channel as an 8-bit integer (from 0 to 255).
   *
   * @param int  The channel to return.
   * @returns    The intensity of the specified channel.
   */
  int getAsInt(Channel);

  /**************************************************************************\
   * Marshalling                                                            *
  \**************************************************************************/
  void save(DOMNodeWriter*);

  

  void debug();
};

#endif
