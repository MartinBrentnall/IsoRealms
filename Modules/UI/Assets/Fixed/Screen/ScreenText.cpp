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
#include "ScreenText.h"

namespace IsoRealms::UI {
  ScreenText::ScreenText(const Metadata& /*metadata*/, IResourceData& owner) :
            cDefString(owner),
            cDefFont(owner),
            cDefColour(owner, 1.0f, 1.0f, 1.0f),
            cDefAlignment(IFont::Alignment::LEFT),
            cDefShadowOffset(DEFAULT_SHADOW_OFFSET) {
  }
  
  ScreenText::ScreenText(const Metadata& metadata, IResourceData& owner, JSONObject object) :
            ScreenText(metadata, owner) {
    std::string mAlignment = object.getString(JSON_ALIGNMENT);
    setAlignment(mAlignment);
    cDefString.set(object, JSON_VALUE);
    cDefFont.set(object, JSON_FONT);
    cDefColour.set(object, JSON_COLOUR);
    cDefShadowOffset = object.getFloat(JSON_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
  }

  void ScreenText::renderScreen(float scale, float aspectRatio) const {
    float mX = cDefAlignment == IFont::Alignment::LEFT  ? -aspectRatio
             : cDefAlignment == IFont::Alignment::RIGHT ?  aspectRatio
             :                                             0.0f;
    Utils::shadowPrint(mX, -1.0f, **cDefFont, 1.0f, **cDefColour, cDefShadowOffset, cDefAlignment, cDefString->getValue());
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  bool ScreenText::renderAssetIcon() const {
    return false;
  }
  
  void ScreenText::saveAsset(JSONObject object) const {
    object.addString(JSON_ALIGNMENT, getAlignment());
    cDefString.save(object, JSON_VALUE);
    cDefFont.save(object, JSON_FONT);
    cDefColour.save(object, JSON_COLOUR);
    object.addFloat(JSON_SHADOW_OFFSET, cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
  }

  void ScreenText::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_VALUE,         cDefString);
    owner.createPropertyTreeSelector(JSON_FONT,          cDefFont);
    owner.createPropertyList(        JSON_ALIGNMENT,     std::vector<std::string>{ALIGNMENT_CENTER, ALIGNMENT_LEFT, ALIGNMENT_RIGHT}, [this]() {return getAlignment();}, [this](const std::string& value) {setAlignment(value);});
    owner.createPropertyTreeSelector(JSON_COLOUR,        cDefColour);
    owner.createPropertyNativeFloat( JSON_SHADOW_OFFSET, [this]() {return cDefShadowOffset;}, [this](float value) {cDefShadowOffset = value;});
  }
  
  bool ScreenText::isDefaultConfiguration() const {
    return cDefString.isDefaultConfigured() && cDefFont.isDefaultConfigured() && cDefAlignment == IFont::Alignment::LEFT && cDefColour.isDefaultConfigured() && cDefShadowOffset == DEFAULT_SHADOW_OFFSET; // TODO: Is this right?
  }

  std::string ScreenText::getAlignment() const {
    return cDefAlignment == IFont::Alignment::LEFT  ? ALIGNMENT_LEFT
         : cDefAlignment == IFont::Alignment::RIGHT ? ALIGNMENT_RIGHT
         :                                            ALIGNMENT_CENTER;
  }
  
  void ScreenText::setAlignment(const std::string& alignment) {
      cDefAlignment = alignment == ALIGNMENT_LEFT  ? IFont::Alignment::LEFT
                    : alignment == ALIGNMENT_RIGHT ? IFont::Alignment::RIGHT
                    :                                IFont::Alignment::CENTER;
  }
}
