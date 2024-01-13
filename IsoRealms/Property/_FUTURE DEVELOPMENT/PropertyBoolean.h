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
  class PropertyBoolean : public PropertyAssetBase<IBoolean> {
    public:
    enum class Type {
      TRUE_AND_FALSE,
      ONLY_FALSE,
      ONLY_TRUE
    };
    
    public:
    PropertyBoolean(const std::string& label, IBoolean** asset, IAssetUser<IBoolean>* client, IAssetBrowser* browser, Type mode = TRUE_AND_FALSE, const std::string& firstLabel = "False", const std::string& secondLabel = "True");

    /****************************\
     * Implements PropertyAsset *
    \****************************/
    IBoolean* getAsset(IAssetBrowser* browser, IAssetUser<IBoolean>* client, const std::string& id) const override;
    bool renderIcon(IAssetBrowser* browser, const std::string& id) const override;

    private:
    GlobalOption cFalseOption;
    GlobalOption cTrueOption;
  };
}
