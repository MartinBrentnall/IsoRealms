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

#include "Modules/Equilibria/Resources/Client/BoundaryType.h"
#include "Modules/Equilibria/Resources/Client/PhysicalObjectType.h"

namespace IsoRealms::Equilibria {
  class BoundaryHandlerInstance;
  class Equilibria;
  class World;

  /**
   * Component definition triggering actions when objects cross boundaries.
   * For each boundary handler, an entry and exit action may be configured
   * upon an object of a specified type crossing a boundary of a specified
   * type.
   */
  class BoundaryHandler final : public IEventBindings {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    BoundaryHandler(Equilibria& equilibria, IComponentData& data);
    void define(IComponentDefiner& definer);
    void removed();

    // Boundary handler interface.
    const BoundaryType* getBoundaryType() const;
    const PhysicalObjectType* getObjectType() const;
    std::unique_ptr<BoundaryHandlerInstance> createInstance(World* world);
    void executeAction(bool exited);

    /*****************************\
     * Implements IEventBindings *
    \*****************************/
    IBinding* getBinding(const std::string& id) override;
    std::string getBindingID(const IBinding* binding) const override;
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    void releaseBinding(const IBinding* resource) override;
    
    private:

    inline static const std::string BIND_TO_BOUNDARY = "Boundary";
    inline static const std::string BIND_TO_OBJECT   = "Object";

    // External interfaces.
    Equilibria& cEquilibria;
    
    // Action client.
    ActionContext cActionContext;

    // Definition data.
    BoundaryType cDefBoundaryType;     /// Boundary type to handle.
    PhysicalObjectType cDefObjectType; /// Object type to handle.
    Action cDefEnteredAction;          /// Action to execute upon object entering a boundary.
    Action cDefExitedAction;           /// Action to execute upon object leaving a boundary.
  };
}

