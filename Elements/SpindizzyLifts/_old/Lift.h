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
#ifndef LIFT_H
#define LIFT_H

#include "Configuration.h"
#include "Point.h"
#include "TextureSet.h"
#include "SpindizzyConstants.h"
#include "SwitchLogic.h"
#include "PlaneSurface.h"
#include "SoundSet.h"

// TODO: implements resettable?..... hmm.
class Lift:public SpindizzyConstants, public PlaneSurface {
  private:
  /**************************************************************************\
   * Lift staten                                                            *
  \**************************************************************************/
  /** Lift is waiting at its highest point. */
  static const int PAUSE_TOP = 0;

  /** Lift is waiting at its lowest point. */
  static const int PAUSE_BOTTOM = 1;

  /** Lift is moving up. */
  static const int MOVING_UP = 2;

  /** Lift is moving down. */
  static const int MOVING_DOWN = 3;

  /**************************************************************************\
   * Global lift configuration fields                                       *
  \**************************************************************************/
  /** Switch states used to control activation and deactivation of lifts. */
  static SwitchLogic* cSwitchLogic;

  /** Textures that lifts use for rendering. */
  static TextureSet* cTextures;

  /** Sounds that lifts makes when moving. */
  static SoundSet* cSoundSet;

  /**************************************************************************\
   * Fields                                                                 *
  \**************************************************************************/
  /** Position of the lift, including its initial height. */
  Point cLocation;

  /** Highest position of the lift. */
  int cHighZPoint;

  /** Lowest position of the lift. */
  int cLowZPoint;

  /** Number of ticks that the lift will wait at its lowest point before moving up. */
  int cDownDelay;

  /** Number of ticks that the lift will wait at its highest point before moving down. */
  int cUpDelay;

  /** Previous height of the lift.  It's used for detecting when we need to play sounds. */
  float cPreviousZPoint;

  /** The current dynamic height of the lift.  Changes as the lift moves. */
  float cCurrentZPoint;

  /** Number of ticks until the lift begins moving again. */
  float cDelayUntilMove;

  /** The current dynamic state of this lift. */
  int cState;
 
  /** The type of this lift; corresponds to a switch type used to activate it. */
  int cType;  

  /**************************************************************************\
   * Global lift functions                                                  *
  \**************************************************************************/
  /**
   * Plays one of the available lift sounds.
   */
  static void playClick();

  /**************************************************************************\
   * Private rendering methods                                              *
  \**************************************************************************/
  /**
   * Render arrow markers to show the range of movement for this lift.
   */
  void renderArrowLines();

  /**
   * Using a line between the two specified Coordinates, returns the point at
   * which the line crosses the X,Y boundary of the lift.  If the line does
   * not cross the boundary of the lift, NULL is returned.
   *
   * @param  Coordinate&  Start point.
   * @param  Coordinate&  End point.
   * @returns             The Coordinate at which the line crosses the boundary
   *                      of this surface, or NULL if it doesn't cross.
   */
  Coordinate* getBoundaryCrossingPoint(Coordinate&, Coordinate&, float, float*);

  /**************************************************************************\
   * Internal movement calculation and prediction                           *
  \**************************************************************************/
  /**
   * Get the Z location of the lift after the specified number of ticks.  This
   * is useful when we want to anticipate a future lift position without
   * actually moving the lift.
   *
   * @param float   Number of ticks in the future.
   * @param int*    Pointer to a lift state.
   * @param float*  Pointer to a value to delay until movement begins.
   * @param bool    true to play movement sound. 
   * @return        Z location of the lift at the specified number of ticks in
   *                the future.
   */
  float getZLocationAfter(float, int*, float*, bool);

  public:
  /**************************************************************************\
   * Construction                                                           *
  \**************************************************************************/
  /**
   * Create a lift at 0,0,0 that doesn't move, of the specified type.
   *
   * @param int  Type of this lift, corresponding to the switch that should
   *             be used to activate it.  A type of NONE results in a lift
   *             that always moves regardless of switch states.
   */
  Lift(int);

  /**************************************************************************\
   * Global lift configuration                                              *
  \**************************************************************************/
  /**
   * Initialise global configuration.
   */
  static void init();

  /**************************************************************************\
   * Individual lift configuration                                          *
  \**************************************************************************/
  /**
   * Set the start location of this lift to the specified point.
   *
   * @param Point&  Destination start location of this lift.
   */
  void setLocation(Point& location);

  /**
   * Set the up height of this lift.
   *
   * @param int  The highest this lift will go.
   */
  void setHighPoint(int height);

  /**
   * Set the down height of this lift.
   *
   * @param int  The lowest this lift will go.
   */
  void setLowPoint(int height);

  /**
   * Set the amount of time for the lift to pause when it reaches its highest
   * position in milliseconds, before it begins moving down.
   *
   * @param int  Up height delay in milliseconds.
   */
  void setUpDelay(int milliseconds);

  /**
   * Set the amount of time for the lift to pause when it reaches its lowest
   * position in milliseconds, before it begins moving up.
   *
   * @param int  Down height delay in milliseconds.
   */
  void setDownDelay(int milliseconds);


  /**************************************************************************\
   * Lift dynamics                                                          *
  \**************************************************************************/
  /**
   * Update the lift dynamics by the specified number of ticks.  The lift may
   * move either up or down, or stay still depending on its attributes and
   * the current switch states.
   *
   * @param float  Numebr of ticks to update by.
   */
  void updateLocation(float ticks);

  /**
   * Reset the lift back to its initial position.
   */
  void resetToOrigin();

  /**************************************************************************\
   * Rendering                                                              *
  \**************************************************************************/
  /**
   * Render the lift as an editable object, at the specified alpha level.
   * This method will always draw the lift at its original location, even
   * if it has moved up or down.  Arrows are also drawn to show the movement
   * range of the lift.
   *
   * @param float  Level of alpha transparency used to render the lift.
   */
  void render(float);

  /**
   * Render the lift as a dynamic object, at the specified alpha level.
   * The lift will be drawn at it's current location, without any markings.
   *
   * @param float  Level of alpha transparency used to render the lift.
   */
  void renderAsDynamic(float alpha);

  /**************************************************************************\
   * Marshalling                                                            *
  \**************************************************************************/
  /**
   * Write this lift to the specified file.
   *
   * @param FILE*  The file to write this lift to.
   */
  void write(FILE *w);

  /**************************************************************************\
   * Implemented methods of PlaneSurface.h                                  *
  \**************************************************************************/
  int getXSlopeAt(Coordinate& location);
  int getYSlopeAt(Coordinate& location);
  int getNorth();
  int getSouth();
  int getEast();
  int getWest();
  float getHeightAt(Coordinate& point);
  bool isVisible();
  void render(TextureSet& textures);
  bool within(int x, int y);
  bool isCeiling();
  bool isVerticallyAligned(Coordinate&);
  bool isBelow(Point& point);
  bool isDynamic();
  ImpactPoint* getRollingEvent(Coordinate&, Coordinate&, float);
  ImpactPoint* getCollisionEvent(Coordinate&, Coordinate&, float);
};

#endif
