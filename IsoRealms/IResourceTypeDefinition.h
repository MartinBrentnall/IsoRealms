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

namespace IsoRealms {
  class DOMNode;
  class IAssetRegistry;
  class IAssets;
  class IAssetLiterals;
  class IAssetRemover;
  class IProject;
  class IOptions;
  class IResource;
  class IResourceType;
  class ModuleOptions;

  class IResourceTypeDefinition {
    public:
    virtual IResource* createResource(IResourceType* parent, IProject* project, IAssetRegistry* registry, const std::string& name) = 0;
    virtual IResource* loadResource(IResourceType* parent, IProject* project, IAssetRegistry* registry, DOMNode& node, IOptions* options) = 0;
    virtual void deleteResource(IAssetRemover* assets, IAssets* releaser, IResource* resource) = 0;
  };
}
