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
#ifndef BLOCK_LOCATION_H
#define BLOCK_LOCATION_H

#include <string>

#include "Persistence/DOMNodeWrapper.h"
#include "Persistence/DOMNodeWriter.h"
#include "Utils.h"

/**
 * A Point represents an absolute location within a map, to block level
 * accuracy.
 */
class BlockLocation {
  public:

  /** Absolute X location of this point. */
  int x;

  /** Absolute Y location of this point. */
  int y;

  /** Absolute Z location of this point. */
  int z;

  public:
  /**
   * Construct a point at 0,0,0.
   */
  BlockLocation();

  /**
   * Construct a point at the specified location.
   *
   * @param int  Absolute X location of the point.
   * @param int  Absolute Y location of the point.
   * @param int  Absolute Z location of the point.
   */
  BlockLocation(int, int, int);

  /**
   * Test this point with the specified point to see whether both points are
   * at the same location.
   *
   * @param BlockLocation&  The point to test against.
   * @returns       true if both points are at the same location.
   */
  bool operator==(BlockLocation& BlockLocation);

  /**
   * Set the location of this point to the location of the specified point.
   *
   * @param BlockLocation  The point to set the location of this point to.
   */
  void set(BlockLocation);

  void set(DOMNodeWrapper*, const std::string& = "x", const std::string& = "y", const std::string& = "z");

  void setRelative(DOMNodeWrapper*, BlockLocation&, const std::string& = "x", const std::string& = "y", const std::string& = "z");

  /**
   * Set the location of this point.
   *
   * @param int  Absolute X location of the point.
   * @param int  Absolute Y location of the point.
   * @param int  Absolute Z location of the point.
   */
  void set(int, int, int);

  void save(DOMNodeWriter*, const std::string& = "x", const std::string& = "y", const std::string& = "z");

  void saveRelative(DOMNodeWriter*, BlockLocation&, const std::string& = "x", const std::string& = "y", const std::string& = "z");

  void debug();
};

#endif
