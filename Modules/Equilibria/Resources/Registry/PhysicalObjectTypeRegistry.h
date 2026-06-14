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

#include "IsoRealms.h"

#include "Modules/Equilibria/Resources/Type/IPhysicalObjectType.h"

namespace IsoRealms::Equilibria {
  class Equilibria;
  
  class PhysicalObjectTypeRegistry : public ResourceClientManager<PhysicalObjectTypeRegistry, Equilibria, IPhysicalObjectType> {
    public:
    PhysicalObjectTypeRegistry();

    private:
    class Dummy : public IPhysicalObjectType {
      public:
    
      /**********************************\
       * Implements IPhysicalObjectType *
      \**********************************/
      std::string getPhysicalObjectTypeID() const override;
      IBinding* getPhysicalObjectTypeBinding(const std::string& id) const override;
      std::string getPhysicalObjectTypeBindingID(const IBinding* binding) const override;
      void forEachAvailablePhysicalObjectTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      bool renderResourceIcon() const override;
    };

    ResourceLiteralDummy<Equilibria, IPhysicalObjectType, Dummy> cDummy;
  };
}
