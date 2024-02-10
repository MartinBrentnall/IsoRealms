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

#include "Property/IProperty.h"

namespace IsoRealms {
  class DOMNodeWriter;
  class IEditingContext;
  class IAssetBrowser;
  class IAssetIdentifier;
  class IAssetRemover;
  class IFont;
  class IPropertyAppearance;
  class IPropertyListener;

  class IResource {
    private:
    std::string cResourceDataPath;
    
    public:
    virtual std::vector<IProperty*> getProperties(IAssetBrowser* browser, IPropertyListener* listener) = 0;
    virtual void finishEditing() = 0;
    virtual std::string getName() = 0;
    virtual bool renderIcon() = 0;
    virtual void hintInUse(bool inUse) = 0;
    virtual void save(DOMNodeWriter* node, IAssetIdentifier* identifier) = 0;
    virtual void registerAssets() = 0;
    virtual void unregisterAssets(IAssetRemover* assets, IAssets* releaser) = 0;

    std::string getResourceDataPath() const {
      return cResourceDataPath;
    }
    
    void setResourceDataPath(const std::string& resourceDataPath) {
      cResourceDataPath = resourceDataPath;
    }
    
    virtual ~IResource() {}
  };
}
