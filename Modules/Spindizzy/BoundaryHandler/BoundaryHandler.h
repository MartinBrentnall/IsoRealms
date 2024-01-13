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

#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Client/BoundaryType.h"
#include "Modules/Spindizzy/Assets/Client/PhysicalObjectType.h"

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
  class BoundaryHandler final : public IBindingRegistry {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    BoundaryHandler(IProject* project, Spindizzy* spindizzy);
    BoundaryHandler(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    // Boundary handler interface.
    const BoundaryType* getBoundaryType() const;
    const PhysicalObjectType* getObjectType() const;
    std::unique_ptr<BoundaryHandlerInstance> createInstance(World* world);
    void executeAction(bool exited);

    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void releaseBinding(const IBinding* asset) override;
    
    private:

    // DOM strings.
    static const std::string TAG_BOUNDARY_TYPE;
    static const std::string TAG_ENTERED_ACTION;
    static const std::string TAG_EXITED_ACTION;
    static const std::string TAG_OBJECT_TYPE;
    
    static const std::string BIND_TO_BOUNDARY;
    static const std::string BIND_TO_OBJECT;

    // Definition data.
    BoundaryType cDefBoundaryType;     /// Boundary type to handle.
    PhysicalObjectType cDefObjectType; /// Object type to handle.
    Action cDefEnteredAction;          /// Action to execute upon object entering a boundary.
    Action cDefExitedAction;           /// Action to execute upon object leaving a boundary.
  };
}

