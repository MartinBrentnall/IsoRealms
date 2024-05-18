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
#include "LiteralFont.h"

namespace IsoRealms {
  void LiteralFont::print(float x, float y, float size, IFont::Alignment Alignment, const std::string& text) {
    // Dummy implementation, do nothing
  }

  float LiteralFont::getWidth(float size, const std::string& text) {
    return 0.0f;
  }

  float LiteralFont::getHeight(float size, const std::string& text) {
    return 0.0;
  }

  unsigned int LiteralFont::getChar(float position, float size, const std::string& text) {
    return 0;
  }

  bool LiteralFont::renderAssetIcon() const {
    return false;
  }

  void LiteralFont::saveAsset(JSONObject object) const {
    // Nothing to do.
  }
}

