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
#include "MenuItemSlider.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  MenuItemSlider::MenuItemSlider(const Metadata& metadata, Menu& menu) :
            cHatHandler(menu.getResourceData().getProject().getApplication().getHatHandler()),
            cMenu(menu),
            cDefID(""),
            cDefLabel(""),
            cDefMinimum(DEFAULT_MINIMUM),
            cDefMaximum(DEFAULT_MAXIMUM),
            cDefSteps(DEFAULT_STEPS),
            cDefValueChangedAction(menu.getResourceData().getDummyActionContext()),
            cLuaBinding(menu.getResourceData().getProject().getLuaState(), this) {
  }

  MenuItemSlider::MenuItemSlider(const Metadata& metadata, Menu& menu, JSONObject object) :
            MenuItemSlider(metadata, menu) {
  }

  void MenuItemSlider::setValue(float value) {
    cRuntimeValue = value;
  }

  float MenuItemSlider::getValue() {
    return cRuntimeValue;
  }

  void MenuItemSlider::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "Menu Items/Sliders");
  }
  
  void MenuItemSlider::reset() {
    cRuntimeValue = cDefMinimum;
  }
  
  bool MenuItemSlider::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Left:  adjustValue(-(cDefMaximum - cDefMinimum) / cDefSteps); return true;
          case sf::Keyboard::Right: adjustValue( (cDefMaximum - cDefMinimum) / cDefSteps); return true;
          default:                                                                         break;
        }
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.leftPressed())  {adjustValue(-(cDefMaximum - cDefMinimum) / cDefSteps); return true;}
        if (cHatHandler.rightPressed()) {adjustValue( (cDefMaximum - cDefMinimum) / cDefSteps); return true;}
        break;
      }

      default: break;
    }
    return false;
  }

  void MenuItemSlider::selectTop() {
    // Nothing to do.
  }

  void MenuItemSlider::selectBottom() {
    // Nothing to do.
  }

  void MenuItemSlider::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    const Font& mFont = menu.getFont();
    float mFontSize = menu.getFontSize();
    float mShadowOffset = menu.getShadowOffset();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    const IColour& mColour = selected ? static_cast<const IColour&>(**menu.getSelectionColour())
                                      : static_cast<const IColour&>(mWhite);
    Utils::shadowPrint(-aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::LEFT,  cDefLabel);
    
    // Render the slider itself.
    float mLineBottom    = y + mFontSize * 1.2f;
    float mLineTop       = mLineBottom + mFontSize * 0.4f;
    float mDimFactor     = 0.2f;
    float mRight         = aspectRatio - mShadowOffset;
    float mValueLocation = mRight * ((cDefMinimum + cRuntimeValue) / (cDefMaximum - cDefMinimum));
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(mRight + mShadowOffset, mLineBottom - mShadowOffset, 0.0f);
    glVertex3f(mRight + mShadowOffset, mLineTop    - mShadowOffset, 0.0f);
    glVertex3f(         mShadowOffset, mLineTop    - mShadowOffset, 0.0f);
    glVertex3f(         mShadowOffset, mLineBottom - mShadowOffset, 0.0f);
    mColour.set();
    glVertex3f(mValueLocation, mLineBottom, 0.0f);
    glVertex3f(mValueLocation, mLineTop,    0.0f);
    glVertex3f(0.0f,           mLineTop,    0.0f);
    glVertex3f(0.0f,           mLineBottom, 0.0f);
    glColor3f(mColour.getRed() * mDimFactor, mColour.getGreen() * mDimFactor, mColour.getBlue() * mDimFactor);
    glBegin(GL_QUADS);
    glVertex3f(mRight,         mLineBottom, 0.0f);
    glVertex3f(mRight,         mLineTop,    0.0f);
    glVertex3f(mValueLocation, mLineTop,    0.0f);
    glVertex3f(mValueLocation, mLineBottom, 0.0f);
    glEnd();
  }

  float MenuItemSlider::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f;
  }

  float MenuItemSlider::getSelectedY(const Menu& menu) const {
    return 0.0f;
  }

  std::string MenuItemSlider::getTreeItemLabel() const {
    return cDefLabel;
  }

  bool MenuItemSlider::renderAssetIcon() const {
    return false;
  }

  void MenuItemSlider::saveAsset(JSONObject object) const {
    object.addString(JSON_ID, cDefID);
    object.addString(JSON_LABEL, cDefLabel);
    object.addFloat(JSON_MINIMUM, cDefMinimum, DEFAULT_MINIMUM);
    object.addFloat(JSON_MAXIMUM, cDefMaximum, DEFAULT_MAXIMUM);
    object.addInteger(JSON_STEPS, cDefSteps, DEFAULT_STEPS);
    cDefValueChangedAction.save(object, JSON_ON_CHANGE);
  }

  void MenuItemSlider::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeString( JSON_ID,        [this]() {return cDefID;},      [this](const std::string& value) {
      cDefID = value;
      cMenu.getResourceData().reregisterAssets();
    });
    owner.createPropertyNativeString( JSON_LABEL,     [this]() {return cDefLabel;},   [this](const std::string& value) {cDefLabel   = value;});
    owner.createPropertyNativeFloat(  JSON_MINIMUM,   [this]() {return cDefMinimum;}, [this](float              value) {cDefMinimum = value;});
    owner.createPropertyNativeFloat(  JSON_MAXIMUM,   [this]() {return cDefMaximum;}, [this](float              value) {cDefMaximum = value;}, DEFAULT_MAXIMUM);
    owner.createPropertyNativeInteger(JSON_STEPS,     [this]() {return cDefSteps;},   [this](int                value) {cDefSteps   = value;}, DEFAULT_STEPS);
    owner.createPropertyTreeSelector( JSON_ON_CHANGE, cDefValueChangedAction);
  }

  bool MenuItemSlider::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }

  void MenuItemSlider::adjustValue(float amount) {
    float mNewValue = std::clamp(cRuntimeValue + amount, cDefMinimum, cDefMaximum);
    if (mNewValue != cRuntimeValue) {
      cRuntimeValue = mNewValue;
      cDefValueChangedAction.execute();
    }
  }
}
