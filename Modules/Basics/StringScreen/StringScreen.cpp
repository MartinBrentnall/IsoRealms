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
#include "StringScreen.h"

#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms::Basics {
  const std::string StringScreen::TAG_COLOUR = "Colour";
  const std::string StringScreen::TAG_FONT   = "Font";
  const std::string StringScreen::TAG_VALUE  = "Value";

  const std::string StringScreen::ATTRIBUTE_ALIGNMENT     = "alignment";
  const std::string StringScreen::ATTRIBUTE_SHADOW_OFFSET = "shadowOffset";
  
  const std::string StringScreen::ALIGNMENT_CENTER = "Center";
  const std::string StringScreen::ALIGNMENT_LEFT   = "Left";
  const std::string StringScreen::ALIGNMENT_RIGHT  = "Right";
  
  const float StringScreen::DEFAULT_SHADOW_OFFSET = 0.2f;
    
  StringScreen::StringScreen(IProject* project, Basics* basics) :
            cDefString(project),
            cDefFont(project),
            cDefColour(project, 1.0f, 1.0f, 1.0f) {
  }

  StringScreen::StringScreen(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            StringScreen(project, basics) {
    std::string mAlignment = node.getAttribute(ATTRIBUTE_ALIGNMENT);
    cDefAlignment = mAlignment == ALIGNMENT_LEFT  ? IFont::Alignment::LEFT
                  : mAlignment == ALIGNMENT_RIGHT ? IFont::Alignment::RIGHT
                  :                                 IFont::Alignment::CENTER;
    cDefString.init(node, TAG_VALUE);
    cDefFont.init(node, TAG_FONT);
    cDefColour.init(node, TAG_COLOUR);
    cDefShadowOffset = node.getFloatAttribute(ATTRIBUTE_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
  }

  void StringScreen::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "String Screens");
  }
  
  void StringScreen::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void StringScreen::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_ALIGNMENT, cDefAlignment == IFont::Alignment::LEFT  ? ALIGNMENT_LEFT
                                          : cDefAlignment == IFont::Alignment::RIGHT ? ALIGNMENT_RIGHT
                                          :                                            ALIGNMENT_CENTER);
    cDefString.save(node, TAG_VALUE);
    cDefFont.save(node, TAG_FONT);
    cDefColour.save(node, TAG_COLOUR);
    if (cDefShadowOffset != DEFAULT_SHADOW_OFFSET) {
      node->addAttribute(ATTRIBUTE_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
    }
  }

  void StringScreen::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool StringScreen::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> StringScreen::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void StringScreen::renderScreen(float scale, float aspectRatio) const {
    float mX = cDefAlignment == IFont::Alignment::LEFT  ? -aspectRatio
             : cDefAlignment == IFont::Alignment::RIGHT ?  aspectRatio
             :                                             0.0f;
    Utils::shadowPrint(mX, -1.0f, **cDefFont, 1.0f, cDefColour, cDefShadowOffset, cDefAlignment, cDefString->getValue());
  }

  bool StringScreen::renderAssetIcon() const {
    return renderIcon();
  }
}
