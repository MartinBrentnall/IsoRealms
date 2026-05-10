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

#include "IsoRealms/Assets/Type/IBinding.h"

#include "Asset.h"

namespace IsoRealms {
  class IActionContext;
  class BindingType;

  class Binding : public Asset<Binding, IBinding, IActionContext> {
    public:
    Binding(IActionContext& owner);
    Binding(IActionContext& owner, const BindingType* type);
    std::string getType() const;
    std::string getRootFolder() const;

    TreeItemInfo getTreeItemInfo() const override;
    bool renderTreeItemIcon(const std::string& id) const override;

    /**********************************************\
     * Implements Asset<IBinding, IActionContext> *
    \**********************************************/
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    bool hasClientConfiguration() const;

    void getClientProperties(IPropertyMaker& owner) override;

    private:
    const BindingType* cDefType;     /// Pointer to the binding type within the argument definition.
    IEventBindings* cDefRegistry;  /// Pointer to the binding registry.
  };
}
