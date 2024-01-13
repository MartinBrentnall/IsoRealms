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
#include <limits>
#include <set>
#include <stack>
#include <GL/glew.h>

#include "IsoRealms/Types.h"

#include "PropertyAssetBase.h"

namespace IsoRealms {
  class PropertyFloat : public PropertyAssetBase<IFloat> {
    private:
    class FixedFloat : public IAssetMenuItem {
      private:
      PropertyFloat* cParent;
      mutable std::string cValue;
      
      public:
      FixedFloat(PropertyFloat* parent);
        
      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(IPropertyAppearance* appearance, float yOffset) const override;
      bool executeAction() override;
      float getWidth(IPropertyAppearance* appearance) const override;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override;
    } cFixedOption;
    
    public:
    PropertyFloat(const std::string& label, IFloat** asset, IAssetUser<IFloat>* client, IAssetBrowser* browser);

    /****************************\
     * Implements PropertyAsset *
    \****************************/
    bool renderIcon(IAssetBrowser* browser, const std::string& id) const override;
    IFloat* getAsset(IAssetBrowser* browser, IAssetUser<IFloat>* client, const std::string& id) const override;
  };
}
