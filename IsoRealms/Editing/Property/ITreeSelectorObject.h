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
#include <string>

#include "IsoRealms/Project/Registry/TreeItemInfo.h"

namespace IsoRealms {
  class Application;
  class IPropertyMaker;
  class Metadata;

  class ITreeSelectorObject {
    public:
    virtual ~ITreeSelectorObject() = default;

    virtual std::string getTreeItemLabel() const = 0;
    virtual bool renderAssetIcon() const = 0;
    virtual bool hasConfiguration() const = 0;
    virtual void getAssetProperties(IPropertyMaker& owner) = 0;
    virtual const Metadata& getPropertyMetadata() const = 0;
    virtual TreeItemInfo getTreeItemInfo() const = 0;
    virtual Application& getApplication() = 0;
    virtual void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const = 0;
    virtual bool renderTreeItemIcon(const std::string& id) const = 0;
    virtual bool isDefaultConfigured() const = 0;
    virtual void setID(const std::string& id) = 0;
  };
}
