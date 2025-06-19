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

#include "IsoRealms/Assets/IBindingRegistry.h"
#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Client/BoundaryType.h"
#include "Modules/Spindizzy/Assets/Client/PhysicalObjectType.h"
#include "Modules/Spindizzy/IBindingIdentifier.h"

namespace IsoRealms::Spindizzy {
  class BoundaryHandlerInstance;
  class Spindizzy;
  class World;

  /**
   * Resource definition triggering actions when objects cross boundaries.
   * For each boundary handler, an entry and exit action may be configured
   * upon an object of a specified type crossing a boundary of a specified
   * type.
   */
  class BoundaryHandler final : public IBindingRegistry,
                                public IBindingIdentifier {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    BoundaryHandler(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    BoundaryHandler(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets);

    // Boundary handler interface.
    const BoundaryType* getBoundaryType() const;
    const PhysicalObjectType* getObjectType() const;
    std::unique_ptr<BoundaryHandlerInstance> createInstance(World* world);
    void executeAction(bool exited);

    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void saveBinding(JSONObject object, const IBinding* binding) const override;
    void releaseBinding(const IBinding* asset) override;
    
    /*********************************\
     * Implements IBindingIdentifier *
    \*********************************/
    std::string getBindingID(const IBinding* binding) const override;
    
    private:

    // JSON members.
    static const std::string JSON_BOUNDARY;
    static const std::string JSON_OBJECT;
    static const std::string JSON_ON_ENTRY;
    static const std::string JSON_ON_EXIT;

    static const std::string BIND_TO_BOUNDARY;
    static const std::string BIND_TO_OBJECT;

    // External interfaces.
    Spindizzy& cSpindizzy;

    // Definition data.
    BoundaryType cDefBoundaryType;     /// Boundary type to handle.
    PhysicalObjectType cDefObjectType; /// Object type to handle.
    Action cDefEnteredAction;          /// Action to execute upon object entering a boundary.
    Action cDefExitedAction;           /// Action to execute upon object leaving a boundary.
  };
}

