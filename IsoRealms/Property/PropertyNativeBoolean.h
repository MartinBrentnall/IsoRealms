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
#pragma once

#include <cmath>
#include <functional>
#include <limits>
#include <set>
#include <stack>
#include <GL/glew.h>

#include "IsoRealms/Types.h"

#include "PropertyAsset.h"

namespace IsoRealms {
  class PropertyNativeBoolean : public PropertyAsset {
    public:
    PropertyNativeBoolean(IPropertyAppearance* appearance, const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter, const std::string& labelFalse = "False", const std::string& labelTrue = "True");

    /****************************\
     * Implements PropertyAsset *
    \****************************/
    bool renderIcon(IPropertyAppearance* appearance, const std::string& id) const override;

    private:
    static bool cTrue;
    static bool cFalse;
    std::function<bool()> cGetter;
    std::function<void(bool)> cSetter;
      
    class FixedBoolean : public IAssetMenuItem {
      private:
      PropertyNativeBoolean* cParent;
      const std::string cLabel;
      const std::string cValue;
      std::function<void()> cIcon; 
      
      public:
      FixedBoolean(PropertyNativeBoolean* parent, std::function<void()> icon, const std::string& label, const std::string& value);
        
      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(IPropertyAppearance* appearance, float yOffset) const override;
      bool executeAction() override;
      float getWidth(IPropertyAppearance* appearance) const override;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override;
    };
      
    FixedBoolean cFalseOption;
    FixedBoolean cTrueOption;
  };
}
