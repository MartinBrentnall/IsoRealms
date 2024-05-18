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

#include <set>
#include <string>

#include "IAssetRegistry.h"
#include "IModuleInternal.h"
#include "IProject.h"
#include "IResource.h"
#include "IResourceType.h"
#include "IResourceTypeDefinition.h"
#include "LocalAssetRegistry.h"
#include "Options/LocalOptions.h"
#include "Resource.h"

namespace IsoRealms {
  class ResourceType : public IResourceType {
    private:
    static const std::string JSON_ID;

    IResourceTypeDefinition* cResourceType;
    std::set<IResource*> cResources;
    IModuleInternal* cParent;
    std::string cName;
    std::string cCategory;
    LocalAssetRegistry cResourceTypeAssetRegistry;
    
    public:
    ResourceType(IResourceTypeDefinition* resourceType, IModuleInternal* parent, IAssetRegistry* assetRegistry, const std::string& id, const std::string& name, const std::string& category);
    void loadResource(JSONObject object, IProject* project, IOptions* options, const std::string& resourceDataPath);
    bool needsSaving(const std::string& id) const;
    void save(JSONArray& array, IAssetIdentifier* identifier, const std::string& tag);

    /****************************\
     * Implements IResourceType *
    \****************************/
    std::string const getName() const override;
    std::set<IResource*> getResources() override;
    IResource* createResource() override;
    void deleteResource(IResource* resource) override;
    std::string getPath() override;
    std::string getDataPath(bool user) override;
    void makeUserDataDirectory(const std::string& resourceName) override;
    std::string getProjectPathPrefix(bool user) override;
    std::string getCategory() override;
    IAssetRemover* getAssetRemover() override;
    IAssetRegistry* getAssetRegistry() override;

    virtual ~ResourceType();
  };
}
