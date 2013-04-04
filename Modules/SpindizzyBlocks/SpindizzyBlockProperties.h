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
#ifndef SPINDIZZY_BLOCK_PROPERTIES_H
#define SPINDIZZY_BLOCK_PROPERTIES_H

#include <GL/glew.h>

#include <IsoRealms/Condition.h>
#include <IsoRealms/ConditionElement.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/IsoRealmsConstants.h>

/**
 * 
 */
class SpindizzyBlockProperties {
  private:
  int cCornerHeight[2][2];
  bool cSplitNorthWestSouthEast;
  bool cSteppedBottom;
  int cX;
  int cY;
  Condition* cCondition;

  public:
  SpindizzyBlockProperties();

  // Configuration functions
  void raiseSelectedCorner();
  void lowerSelectedCorner();
  void selectNorth();
  void selectSouth();
  void selectEast();
  void selectWest();
  void toggleSplit();
  void toggleSteppedBottom();
  void renderConfiguration();

  // Automatic configuration functions
  void reset();
  void setup(DOMNodeWrapper*, std::vector<ConditionElement*>);

  // Block preparation functions
  int getNorthWestHeight();
  int getNorthEastHeight();
  int getSouthWestHeight();
  int getSouthEastHeight();
  bool isSplitNorthWestSouthEast();
  bool isSteppedBottom();
  Condition* getCondition();

  // Convenience functions
  /**
   * Return the stepping of the slope along the X axis.  A negative value
   * indicates sloping towards the east; a positive value indicates sloping
   * towards the west.  A value of zero indicates no sloping along the X axis.
   *
   * @returns  Slope along the X axis.
   */ 
  int getXSlope();

  /**
   * Return the stepping of the slope along the Y axis.  A negative value
   * indicates sloping towards the north; a positive value indicates sloping
   * towards the south.  A value of zero indicates no sloping along the Y axis.
   *
   * @returns  Slope along the Y axis.
   */ 
  int getYSlope();

};

#endif
