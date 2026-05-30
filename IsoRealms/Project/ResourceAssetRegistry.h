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
 * You should have received a copy of the GNU General Public License
 *
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>
#include <variant>
 
#include "IsoRealms/Project/Registry/IAssetProvider.h"
#include "IsoRealms/AbstractAssetRegistry.h"
#include "IsoRealms/Types.h"
 
namespace IsoRealms {
  using AssetVariant = std::variant<IAssetProvider<IActionContext, IAction>*,
                                    IAssetProvider<IResourceData, IAnalogueInputMapping>*,
                                    IAssetProvider<IActionContext, IBinding>*,
                                    IAssetProvider<IResourceData, IBindingType>*,
                                    IAssetProvider<IResourceData, IBoolean>*,
                                    IAssetProvider<IResourceData, IColour>*,
                                    IAssetProvider<IResourceData, IDigitalInputMapping>*,
                                    IAssetProvider<IResourceData, IEditable>*,
                                    IAssetProvider<IResourceData, IFont>*,
                                    IAssetProvider<IResourceData, IFloat>*,
                                    IAssetProvider<IResourceData, IInputHandler>*,
                                    IAssetProvider<IResourceData, IInteger>*,
                                    IAssetProvider<IResourceData, IModel>*,
                                    IAssetProvider<IResourceData, IScreen>*,
                                    IAssetProvider<IResourceData, IString>*,
                                    IAssetProvider<IResourceData, ITexture>*,
                                    IAssetProvider<IResourceData, IVertex>*,
                                    IAction*,
                                    IAnalogueInputMapping*,
                                    IBinding*,
                                    IBindingType*,
                                    IBoolean*,
                                    IColour*,
                                    IDigitalInputMapping*,
                                    IEditable*,
                                    IFont*,
                                    IFloat*,
                                    IInputHandler*,
                                    IInteger*,
                                    IModel*,
                                    IScreen*,
                                    IString*,
                                    ITexture*,
                                    IVertex*>;
  class ResourceAssetRegistry : public AbstractAssetRegistry<AssetVariant, IResourceData> {
    public:
    ResourceAssetRegistry(IResourceData& manager) :
              AbstractAssetRegistry<AssetVariant, IResourceData>(manager) {
    }

    ~ResourceAssetRegistry() {
      clear();
    }
  };
}
 