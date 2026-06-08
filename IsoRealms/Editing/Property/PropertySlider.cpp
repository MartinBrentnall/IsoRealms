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
#include "PropertySlider.h"

#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Editing/UISignalID.h"
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  PropertySlider::PropertySlider(IPropertyMaker& owner, const std::string& key, const Metadata& metadata, const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<float()> valueFunction, float minimum, float maximum, std::function<void(const float)> confirmationCallback, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cPropertyOwner(owner),
            cPropertyKey(key),
            cPropertyMetadata(metadata),
            cConfirmationCallback(confirmationCallback),
            cValueFunction(valueFunction),
            cMininum(minimum),
            cMaximum(maximum) {
  }
  
  void PropertySlider::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    float mFontSize = style.getFontSize();
    renderSlider(mFontSize, WIDTH, y, x, aspectRatio);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(x + WIDTH * cValueFunction(), y + mFontSize * 0.5f);
    glVertex2f(x + WIDTH * cValueFunction(), y + mFontSize * 1.5f);
    glEnd();
    Utils::renderCurve(x + WIDTH * cValueFunction(), y + mFontSize * 0.5f, mFontSize * 0.1f, 0.0f, 1.0f);
    Utils::renderCurve(x + WIDTH * cValueFunction(), y + mFontSize * 1.5f, mFontSize * 0.1f, 0.0f, 1.0f);
  }
  
  float PropertySlider::getValueWidth(IUIStyle& style) const {
    return WIDTH;
  }

  void PropertySlider::confirm(IPropertyManager& manager, float y) {
    manager.edit(std::make_unique<Editor>(*this));
  }
  
  bool PropertySlider::hasConfiguration() const {
    return true;
  }
  
  void PropertySlider::configure(IPropertyManager& manager) {
    manager.openProperties(cPropertyOwner.getResourceData(), getPropertyName(), cPropertyMetadata, [this](IPropertyMaker& owner) {
      owner.createPropertyNativeFloat(cPropertyKey, [this]() {
        return cValueFunction();
      }, [this](float value) {
        cConfirmationCallback(value);
      });
    });
  }

  PropertySlider::Editor::Editor(PropertySlider& parent) :
            cParent(parent),
            cOriginalValue(cParent.cValueFunction()) {
  }

  void PropertySlider::Editor::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    cParent.renderValue(style, y, x, aspectRatio);
  }

  bool PropertySlider::Editor::update(unsigned int milliseconds) {
    return true;
  }

  bool PropertySlider::Editor::input(UISignalID id, IUIStyle& style) {
    switch (id) {
      case UISignalID::MOVE_LEFT: {
        cParent.cConfirmationCallback(std::max(cParent.cValueFunction() - 0.05f, cParent.cMininum));
        break;
      }

      case UISignalID::MOVE_RIGHT: {
        cParent.cConfirmationCallback(std::min(cParent.cValueFunction() + 0.05f, cParent.cMaximum));
        break;
      }

      case UISignalID::CANCEL: {
        cParent.cConfirmationCallback(cOriginalValue);
        return true;
      }

      case UISignalID::CONFIRM: {
        cParent.confirmAccess([this]() {
          // Nothing to do.
        }, [this]() {
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

  bool PropertySlider::Editor::input(sf::Event& event, IUIStyle& style) {
    return false;
  }
}

