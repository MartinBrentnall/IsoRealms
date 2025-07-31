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
#include "ScreenGradient.h"

namespace IsoRealms::UI {
  ScreenGradient::ScreenGradient(const Metadata& metadata, IResourceData& owner) :
            cMetadata(metadata),
            cDefColourA(owner, 0.0f, 0.0f, 1.0f),
            cDefColourB(owner, 0.0f, 1.0f, 0.0f),
            cDefVertical(false) {
  }

  ScreenGradient::ScreenGradient(const Metadata& metadata, IResourceData& owner, JSONObject object) :
            ScreenGradient(metadata, owner) {
    cDefColourA.set(object, JSON_COLOUR_A);
    cDefColourB.set(object, JSON_COLOUR_B);
    cDefVertical = object.getString(JSON_ORIENTATION) == VALUE_VERTICAL;
  }

  void ScreenGradient::renderScreen(float scale, float aspectRatio) const {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    cDefColourA->set();
    if (cDefVertical) {
      glVertex2f( aspectRatio,  1.0f);
      glVertex2f(-aspectRatio,  1.0f);
      cDefColourB->set();
      glVertex2f(-aspectRatio, -1.0f);
      glVertex2f( aspectRatio, -1.0f);
    } else {
      glVertex2f(-aspectRatio,  1.0f);
      glVertex2f(-aspectRatio, -1.0f);
      cDefColourB->set();
      glVertex2f( aspectRatio, -1.0f);
      glVertex2f( aspectRatio,  1.0f);
    }
    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
  }

  bool ScreenGradient::renderAssetIcon() const {
    return false;
  }

  void ScreenGradient::saveAsset(JSONObject object) const {
    cDefColourA.save(object, JSON_COLOUR_A);
    cDefColourB.save(object, JSON_COLOUR_B);
    object.addString(JSON_ORIENTATION, cDefVertical ? VALUE_VERTICAL : VALUE_HORIZONTAL);
  }

  void ScreenGradient::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyAsset<Colour>( cMetadata.getPropertyData("ColourA"), cDefColourA);
    owner.createPropertyAsset<Colour>( cMetadata.getPropertyData("ColourB"), cDefColourB);
    owner.createPropertyList(          cMetadata.getPropertyData("Orientation"), std::vector<std::string>{
      VALUE_HORIZONTAL, VALUE_VERTICAL
    }, [this]() {
      return cDefVertical ? VALUE_VERTICAL : VALUE_HORIZONTAL;
    }, [this](const std::string& value) {
      cDefVertical = value == VALUE_VERTICAL; return true;
    });
  }

  bool ScreenGradient::isDefaultConfiguration() const {
    return cDefColourA.isDefaultConfigured() && cDefColourB.isDefaultConfigured(); // TODO: This doesn't factor in the custom colours????'
  }

  const std::string ScreenGradient::JSON_COLOUR_A    = "colourA";
  const std::string ScreenGradient::JSON_COLOUR_B    = "colourB";
  const std::string ScreenGradient::JSON_ORIENTATION = "orientation";

  const std::string ScreenGradient::VALUE_HORIZONTAL = "Horizontal";
  const std::string ScreenGradient::VALUE_VERTICAL   = "Vertical";
}

