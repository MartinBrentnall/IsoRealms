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

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Client/PhysicalObjectType.h"

namespace IsoRealms::Spindizzy {
  class CollisionHandlerInstance;
  class Spindizzy;
  class World;

  /**
   * Resource definition triggering actions when objects go into and out of
   * proximity with each other.  For each collision handler, an entry and exit
   * action may be configured to be triggered when an object of a one type
   * goes into/out of proximity of an object of another type.
   */
  class CollisionHandler final {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    CollisionHandler(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    CollisionHandler(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets);

    // Collision handler interface.
    const PhysicalObjectType* getPhysicalObjectTypeA() const;
    const PhysicalObjectType* getPhysicalObjectTypeB() const;
    std::unique_ptr<CollisionHandlerInstance> createInstance(World* world);
    void executeAction(bool exited);

    private:

    // JSON members.
    static const std::string JSON_OBJECT_A;
    static const std::string JSON_OBJECT_B;
    static const std::string JSON_ON_COLLISION;
    static const std::string JSON_ON_PARTING;

    // External interfaces.
    Spindizzy& cSpindizzy;

    // Definition data.
    PhysicalObjectType cDefPhysicalObjectTypeA; /// First object type to handle.
    PhysicalObjectType cDefPhysicalObjectTypeB; /// Second object type to handle.
    Action cDefEnteredAction;                   /// Action to execute upon an object of the first type entering proximity of an object of the second type.
    Action cDefExitedAction;                    /// Action to execute upon an object of the first type leaving proximity of an object of the second type.
  };
}
