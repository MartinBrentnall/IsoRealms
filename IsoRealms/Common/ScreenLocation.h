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
   * Screen location represents a resolution independent location on the
   * screen.  The X and Y values should always lie somewhere between -1.0 and
   * 1.0, with 0.0 being the center of the screen.  If the X and/or Y values
   * are outside of this range, it indicates a location off the screen.
   */
  class ScreenLocation {
    private:
    const float cX; /// X location on screen.
    const float cY; /// Y location on screen.

    public:
      
    /**
     * Construct a screen location.
     * 
     * @param x Horizontal location on the screen.
     * @param y Vertical location on the screen.
     */
    ScreenLocation(const float x, const float y);

    /**
     * Retrieve the X location.
     * 
     * @return X location.
     */
    float getX() const;
    
    /**
     * Retrieve the Y location.
     * 
     * @return Y location.
     */
    float getY() const;
  };
}
