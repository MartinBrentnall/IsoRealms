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

#include "Modules/Spindizzy/Assets/Type/IPhysicalObjectType.h"

#include "IPhysicalObjectTypeListener.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  
  class PhysicalObjectType : public Asset<PhysicalObjectType, IPhysicalObjectType, Spindizzy> {
    public:
    PhysicalObjectType(Spindizzy& spindizzy);

    IBinding* getBinding(const std::string& id) const;
    std::string getBindingID(const IBinding* binding) const;
    void forEachAvailablePhysicalObjectTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const;

    void addNotifyAssetChangedFunction(IPhysicalObjectTypeListener* listener);
    void removeNotifyAssetChangedFunction(IPhysicalObjectTypeListener* listener);
    void notifyAssetChanged(const IPhysicalObjectType* oldAsset, const IPhysicalObjectType* newAsset);

    /****************************************************\
     * Implements Asset<IPhysicalObjectType, Spindizzy> *
    \****************************************************/
    bool isDefaultConfiguration() const;

    private:
    std::vector<IPhysicalObjectTypeListener*> cNotifyAssetChangedListeners;
  };
}
