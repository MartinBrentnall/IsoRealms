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

#include "ISpindizzyRegistry.h"

namespace IsoRealms::Spindizzy {
  class LocalSpindizzyRegistry : public ISpindizzyRegistry {
    public:
    LocalSpindizzyRegistry(ISpindizzyRegistry* registry, const std::string& prefix);
    
    /*********************************\
     * Implements ISpindizzyRegistry *
    \*********************************/
    void add(IBoundaryType*       asset, const std::string& id) override;
    void add(IPhysicalObjectType* asset, const std::string& id) override;
    void add(IWorldEditorTool*    asset, const std::string& id) override;
    
//     void add(IAssetProvider<IBoundaryType>*        provider, const std::string& id) override;
//     void add(IAssetProvider<ICamera>*              provider, const std::string& id) override;
//     void add(IAssetProvider<IPhysicalObjectType>*  provider, const std::string& id) override;
//     void add(IAssetProvider<ISurfacePattern>*      provider, const std::string& id) override;
//     void add(IAssetProvider<IWallPattern>*         provider, const std::string& id) override;
//     void add(IAssetProvider<IWorldEditorTool>*     provider, const std::string& id) override;
//     void add(IAssetProvider<IZoneObjectTypeTrait>* provider, const std::string& id) override;
//     void add(IAssetProvider<IZoneViewType>*        provider, const std::string& id) override;
    void addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id) override;

    private:
    ISpindizzyRegistry* cRegistry;
    std::string cLocalPrefix;
  };
}
