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
 
#include "IsoRealms/Project/Registry/IResourceProvider.h"
#include "IsoRealms/AbstractResourceRegistry.h"
#include "IsoRealms/Types.h"
 
namespace IsoRealms {
  using ResourceVariant = std::variant<IResourceProvider<IActionContext, IAction>*,
                                       IResourceProvider<IComponentData, IAnalogueInput>*,
                                       IResourceProvider<IActionContext, IBinding>*,
                                       IResourceProvider<IComponentData, IBindingType>*,
                                       IResourceProvider<IComponentData, IBoolean>*,
                                       IResourceProvider<IComponentData, IColour>*,
                                       IResourceProvider<IComponentData, IDigitalInput>*,
                                       IResourceProvider<IComponentData, IEditable>*,
                                       IResourceProvider<IComponentData, IFont>*,
                                       IResourceProvider<IComponentData, IFloat>*,
                                       IResourceProvider<IComponentData, IInputHandler>*,
                                       IResourceProvider<IComponentData, IInteger>*,
                                       IResourceProvider<IComponentData, IModel>*,
                                       IResourceProvider<IComponentData, IScreen>*,
                                       IResourceProvider<IComponentData, IString>*,
                                       IResourceProvider<IComponentData, ITexture>*,
                                       IResourceProvider<IComponentData, IVertex>*,
                                       IAction*,
                                       IAnalogueInput*,
                                       IBinding*,
                                       IBindingType*,
                                       IBoolean*,
                                       IColour*,
                                       IDigitalInput*,
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
  class ResourcePublisher : public AbstractResourceRegistry<ResourceVariant, IComponentData> {
    public:
    ResourcePublisher(IComponentData& manager) :
              AbstractResourceRegistry<ResourceVariant, IComponentData>(manager) {
    }

    ~ResourcePublisher() {
      clear();
    }
  };
}
 