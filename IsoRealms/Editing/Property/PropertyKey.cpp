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
#include "PropertyKey.h"

#include "IsoRealms/Resources/Type/IFont.h"
#include "IsoRealms/Editing/IUIStyle.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  PropertyKey::PropertyKey(const PropertyData& data, IComponentAccessManager& resourceAccessManager, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cGetter(getter),
            cSetter(setter) {
  }
  
  void PropertyKey::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cGetter());
  }
  
  float PropertyKey::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cGetter());
  }
  
  void PropertyKey::confirm(IPropertyManager& manager, float y) {
    manager.edit(std::make_unique<Editor>(*this));
  }

  bool PropertyKey::hasConfiguration() const {
    return false;
  }
  
  void PropertyKey::configure(IPropertyManager& manager) {
    // Nothing to do.
  }
  
  PropertyKey::Editor::Editor(PropertyKey& parent) :
            cParent(parent) {
  }

  void PropertyKey::Editor::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x, y + 0.01f, mFontSize, IFont::Alignment::LEFT, "Press key to map...");
  }

  bool PropertyKey::Editor::update(unsigned int milliseconds) {
    return true;
  }

  bool PropertyKey::Editor::input(UISignalID id, IUIStyle& style) {
    return false;
  }

  bool PropertyKey::Editor::input(sf::Event& event, IUIStyle& style) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        cParent.confirmAccess([this, event]() {
          cParent.cSetter(event.key.code);
        }, []() {
          // Nothing to do.
        });
        return true;
      }
      
      default: {
        break;
      }
    }
    return false;
  }
}
