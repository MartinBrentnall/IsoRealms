/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "IsoRealms.h"

namespace IsoRealms::Equilibria {

  /**
   * Interface for a boundary.
   */
  class IBoundary {
    public:

    /**
     * Check whether the specified location is within this boundary.
     *
     * @param location The location to test.
     * @return true if the location is within this boundary, otherwise false.
     */
    virtual bool contains(LiteralVertex& location) const = 0;

    /**
     * Check whether the line formed from start to end enters this boundary.
     * Note that the test is only performed in one direction.
     *
     * @param start Start location of the line to test.
     * @param end End location of the line to test.
     * @return true if the line enters this boundary.
     */
    virtual bool isEntered(LiteralVertex& start, LiteralVertex& end) const = 0;

    /**
     * Check whether the line formed from start to end leaves this boundary.
     * Note that the test is only performed in one direction.
     *
     * @param start Start location of the line to test.
     * @param end End location of the line to test.
     * @return true if the line leaves this boundary.
     */
    virtual bool isExited(LiteralVertex& start, LiteralVertex& end) const = 0;
    
    /**
     * Bind values relating to this boundary.  This is called immediately
     * before a boundary crossing action is performed.
     */
    virtual void bindValues() = 0;

    /**
     * Unbind values relating to this boundary.  This is called immediately
     * after a boundary crossing action is performed.
     */
    virtual void unbindValues() = 0;
  };
}
