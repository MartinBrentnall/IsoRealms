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

#include <string>

namespace IsoRealms {
  
  /**
   * Screen mode represents the fixed dimensions in pixels of a screen.
   */
  class DisplayResolution {
    private:
    const unsigned int cWidth;  /// Width of screen in pixels.
    const unsigned int cHeight; /// Height of screen in pixels.

    public:
      
    /**
     * Construct a screen mode of the specified width and height.
     * 
     * @param width Width of the screen in pixels.
     * @param height Height of the screen in pixels.
     */
    DisplayResolution(const unsigned int width, const unsigned int height);

    /**
     * Retrieve the width of this screen mode in pixels.
     * 
     * @return Width of the screen in pixels.
     */
    unsigned int getWidth() const;
    
    /**
     * Retrieve the height of this screen mode in pixels.
     * 
     * @return Height of the screen in pixels.
     */
    unsigned int getHeight() const;
    
    /**
     * Test whether the specified screen mode is identical to this one.
     * 
     * @param mode The mode to test for equality.
     * @return true if the mode is equal to this one, otherwise false.
     */
    bool operator==(const DisplayResolution& resolution) const;
    
    std::string toString() const;
  };
}
