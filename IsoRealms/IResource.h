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

#include <string>

#include "Editing/Property/IProperty.h"

namespace IsoRealms {
  class File;
  class JSONObject;
  class IEditingContext;
  class IAssetBrowser;
  class IAssetIdentifier;
  class IAssetRemover;
  class IAssets;
  class IFont;

  class IResource {
    public:
    virtual std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser) = 0;
    virtual std::string getName() = 0;
    virtual bool renderIcon() = 0;
    virtual void hintInUse(bool inUse) = 0;
    virtual void save(JSONObject object, IAssetIdentifier& identifier) = 0;
    virtual void registerAssets() = 0;
    virtual void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) = 0;
    virtual std::string getResourceDataPath() const = 0;    
    virtual bool needsSaving(File* savingProject) = 0;
    
    virtual ~IResource() {}
  };
}
