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

#include "Modules/Equilibria/Assets/Client/PhysicalObjectType.h"

namespace IsoRealms::Equilibria {
  class CollisionHandlerInstance;
  class Equilibria;
  class World;

  /**
   * Component definition triggering actions when objects go into and out of
   * proximity with each other.  For each collision handler, an entry and exit
   * action may be configured to be triggered when an object of a one type
   * goes into/out of proximity of an object of another type.
   */
  class CollisionHandler final : public IPhysicalObjectTypeListener {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    CollisionHandler(Equilibria& equilibria, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    // Collision handler interface.
    const PhysicalObjectType* getPhysicalObjectTypeA() const;
    const PhysicalObjectType* getPhysicalObjectTypeB() const;
    std::unique_ptr<CollisionHandlerInstance> createInstance(World* world);
    void executeAction(bool exited);

    /******************************************\
     * Implements IPhysicalObjectTypeListener *
    \******************************************/
    void physicalObjectTypeChanged(const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType) override;

    private:
    
    // External interfaces.
    Equilibria& cEquilibria;

    // Definition data.
    PhysicalObjectType cDefPhysicalObjectTypeA; /// First object type to handle.
    PhysicalObjectType cDefPhysicalObjectTypeB; /// Second object type to handle.
    Action cDefEnteredAction;                   /// Action to execute upon an object of the first type entering proximity of an object of the second type.
    Action cDefExitedAction;                    /// Action to execute upon an object of the first type leaving proximity of an object of the second type.
  };
}
