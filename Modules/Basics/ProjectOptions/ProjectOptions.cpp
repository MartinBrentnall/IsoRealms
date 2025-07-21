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
#include "ProjectOptions.h"

namespace IsoRealms::Basics {
  const std::string ProjectOptions::JSON_ID      = "id";
  const std::string ProjectOptions::JSON_OPTIONS = "options";
  const std::string ProjectOptions::JSON_VALUE   = "value";

  ProjectOptions::ProjectOptions(IProject& project, Basics& basics, IResourceData& data) {
  }
  
  ProjectOptions::ProjectOptions(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            ProjectOptions(project, basics, data) {
    for (JSONObject mOptionsObject : object.getArray(JSON_OPTIONS)) {
      cDefOptions.emplace(std::piecewise_construct, std::forward_as_tuple(mOptionsObject.getString(JSON_ID)), std::forward_as_tuple(data)).first->second.init(mOptionsObject, JSON_VALUE);
    }
  }

  void ProjectOptions::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IProjectOptions>(this, "", "Project Options");
  }
  
  void ProjectOptions::save(JSONObject object) const {
    JSONArray mOptionsArray = object.addArray(JSON_OPTIONS);
    for (const std::pair<const std::string, String>& mOption : cDefOptions) {
      JSONObject mOptionObject = mOptionsArray.addObject();
      mOptionObject.addString(JSON_ID, mOption.first);
      mOption.second.save(mOptionObject, JSON_VALUE);
    }
  }

  void ProjectOptions::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ProjectOptions::renderIcon() {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> ProjectOptions::getProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  Options ProjectOptions::getFixedOptions() {
    std::map<std::string, std::string> mOptions;
    for (const std::pair<const std::string, String>& mOption : cDefOptions) {
      mOptions[mOption.first] = mOption.second->getValue();
    }
    return Options(mOptions);
  }

  bool ProjectOptions::renderAssetIcon() const {
    return false;
  }

  void ProjectOptions::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ProjectOptions::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ProjectOptions::isDefaultConfiguration() const {
    return true;
  }
}
