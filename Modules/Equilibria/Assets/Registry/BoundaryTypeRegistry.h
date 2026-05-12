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
#pragma once

#include <functional>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Type/IBoundaryType.h"

namespace IsoRealms::Equilibria {
  class Equilibria;

  class BoundaryTypeRegistry : public AssetClientManager<BoundaryTypeRegistry, Equilibria, IBoundaryType> {
    public:
    BoundaryTypeRegistry();

    private:
    class Dummy : public IBoundaryType {
      public:
  
      /****************************\
       * Implements IBoundaryType *
      \****************************/
      std::string getBoundaryTypeID() const override;
      IBinding* getBounderyTypeBinding(const std::string& id) const override;
      std::string getBoundaryTypeBindingID(const IBinding* binding) const override;
      void forEachAvailableBoundaryTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
    };

    AssetLiteralDummy<Equilibria, IBoundaryType, Dummy> cDummy;
  };
}
