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

#include "Modules/Equilibria/Resources/Type/IPhysicalObjectType.h"

#include "IPhysicalObjectTypeListener.h"

namespace IsoRealms::Equilibria {
  class Equilibria;
  
  class PhysicalObjectType : public Resource<PhysicalObjectType, IPhysicalObjectType, Equilibria> {
    public:
    PhysicalObjectType(Equilibria& equilibria);

    IBinding* getBinding(const std::string& id) const;
    std::string getBindingID(const IBinding* binding) const;
    void forEachAvailablePhysicalObjectTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const;

    void addNotifyResourceChangedFunction(IPhysicalObjectTypeListener* listener);
    void removeNotifyResourceChangedFunction(IPhysicalObjectTypeListener* listener);
    void notifyResourceChanged(const IPhysicalObjectType* oldResource, const IPhysicalObjectType* newResource);

    private:
    std::vector<IPhysicalObjectTypeListener*> cNotifyResourceChangedListeners;
  };
}
