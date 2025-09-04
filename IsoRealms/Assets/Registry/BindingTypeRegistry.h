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

#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IBindingType.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class BindingTypeRegistry : public AssetClientManager<BindingTypeRegistry, IResourceData, IBindingType> {
    public:
    BindingTypeRegistry(Project& project);

    private:
    class Literal : public IBindingType {
      public:
      Literal(const std::string& type);

      /***********************\
      * Implements IBinding *
      \***********************/
      std::string getBindingTypeID() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      std::string cType;
    };

    class Dummy : public IBindingType {
      public:
      Dummy(IResourceData& owner);

      /***********************\
      * Implements IBinding *
      \***********************/
      std::string getBindingTypeID() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
    };

    AssetLiteralDummy<IResourceData, IBindingType, Dummy> cDummy;
    Literal cActions;
    Literal cBooleans;
    Literal cColours;
    Literal cFloats;
    Literal cFonts;
    Literal cInputHandlers;
    Literal cIntegers;
    Literal cProjects;
    Literal cScreens;
    Literal cStrings;
    Literal cVertices;
  };
}
