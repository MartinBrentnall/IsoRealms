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

#include "IAsset.h"

namespace IsoRealms {
  class IFloat;
  
  /**
   * Interface to a screen asset.  Screen assets simply provide the ability to
   * be rendered.  It is assumed that identity transformation is set in order
   * that the resulting render covers the full screen or window.
   */
  class IScreen : public IAsset {
    public:

    /**
     * Render this screen.
     */
    virtual void renderScreen(float scale, float aspectRatio) const = 0;

    virtual const IFloat* getYaw() const {
      return nullptr;
    }
    
    virtual const IFloat* getPitch() const {
      return nullptr;
    }
  };
}
