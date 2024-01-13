/*
 * Copyright 2023 Martin Brentnall
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
#pragma once

namespace IsoRealms {
  
  /**
   * Screen area represents a resolution independent area on the screen.  The
   * horizontal and vertical values should always lie somewhere between -1.0
   * and 1.0, with 0.0 being the center of the screen.  If the values are
   * outside of this range, it indicates an area off the screen.
   */
  class ScreenArea {
    private:
    const float cLeft;   /// Left of area on screen.
    const float cRight;  /// Right of area on screen.
    const float cBottom; /// Bottom of area on screen.
    const float cTop;    /// Top of area on screen.

    public:
      
    /**
     * Construct a screen area.
     * 
     * @param left Left of area on the screen.
     * @param right Right of area on the screen.
     * @param bottom Bottom of area on the screen.
     * @param top Top of area on the screen.
     */
    ScreenArea(const float left, const float right, const float bottom, const float top);

    /**
     * Retrieve the left of area on the screen.
     * 
     * @return Left of area.
     */
    float getLeft() const;
    
    /**
     * Retrieve the right of area on the screen.
     * 
     * @return Right of area.
     */
    float getRight() const;
    
    /**
     * Retrieve the bottom of area on the screen.
     * 
     * @return Bottom of area.
     */
    float getBottom() const;
    
    /**
     * Retrieve the top of area on the screen.
     * 
     * @return Top of area.
     */
    float getTop() const;
  };
}
