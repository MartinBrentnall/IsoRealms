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
#include "LocalSpindizzyRegistry.h"

namespace IsoRealms::Spindizzy {
  LocalSpindizzyRegistry::LocalSpindizzyRegistry(ISpindizzyRegistry* registry, const std::string& prefix) :
            cRegistry(registry),
            cLocalPrefix(prefix) {
  }
  
  void LocalSpindizzyRegistry::add(IBoundaryType*       asset, const std::string& id) {cRegistry->add(asset, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
  void LocalSpindizzyRegistry::add(IPhysicalObjectType* asset, const std::string& id) {cRegistry->add(asset, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
  void LocalSpindizzyRegistry::add(IWorldEditorTool*    asset, const std::string& id) {cRegistry->add(asset, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
  
//   void LocalSpindizzyRegistry::add(IAssetProvider<IBoundaryType>*        provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<ICamera>*              provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<IPhysicalObjectType>*  provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<ISurfacePattern>*      provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<IWallPattern>*         provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<IWorldEditorTool>*     provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<IZoneObjectTypeTrait>* provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
//   void LocalSpindizzyRegistry::add(IAssetProvider<IZoneViewType>*        provider, const std::string& id) {cRegistry->add(provider, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
  void LocalSpindizzyRegistry::addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id) {cRegistry->addZoneBinding(binding1, binding2, id.empty() ? cLocalPrefix : cLocalPrefix + "/" + id);}
}
