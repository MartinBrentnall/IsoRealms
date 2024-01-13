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
#include "DisplayResolution.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DisplayResolution::DisplayResolution(unsigned int width, unsigned int height):
          cWidth(width),
          cHeight(height) {
  }

  unsigned int DisplayResolution::getWidth() const {
    return cWidth;
  }

  unsigned int DisplayResolution::getHeight() const {
    return cHeight;
  }

  bool DisplayResolution::operator==(const DisplayResolution& mode) const {
    return cWidth == mode.cWidth && cHeight == mode.cHeight;
  }
  
  std::string DisplayResolution::toString() const {
    return Utils::toString(cWidth) + " x " + Utils::toString(cHeight);
  }
}
