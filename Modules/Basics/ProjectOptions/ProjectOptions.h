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

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for Project Options that reference String variables
   * from within the Project.
   */
  class ProjectOptions final : public IProjectOptions {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ProjectOptions(IProject* project, Basics* basics);
    ProjectOptions(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /******************************\
     * Implements IProjectOptions *
    \******************************/
    Options getFixedOptions() override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

    private:

    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_OPTIONS;
    static const std::string JSON_VALUE;

    // Definition data.
    std::map<std::string, String> cDefOptions;
  };
}
