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
#include "ModuleMetadata.h"

#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "PropertyData.h"

namespace IsoRealms {
  ModuleMetadata::ModuleMetadata(Module& module) {
    std::string mModuleName = module.getName();
    std::locale mLocale("");
    std::string mMetadataPath = "Metadata/" + mModuleName + "/" + mModuleName + "." + mLocale.name();
    std::string::size_type mLastExtensionIndex = mMetadataPath.find_last_of('.');
    std::string::size_type mLastDashIndex = mMetadataPath.find_last_of('_');
    std::string::size_type mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                                               ? mLastExtensionIndex
                                               : mLastDashIndex;
    while (!System::fileExists(mMetadataPath + ".json", false) && mLastSeparatorIndex != std::string::npos) {
      mMetadataPath = mMetadataPath.substr(0, mLastSeparatorIndex);
      mLastExtensionIndex = mMetadataPath.find_last_of('.');
      mLastDashIndex = mMetadataPath.find_last_of('_');
      mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                          ? mLastExtensionIndex
                          : mLastDashIndex;
    }

    if (!System::fileExists(mMetadataPath + ".json", false)) {
      throw std::runtime_error("ERROR: Module::getMetadataPath: No metadata file found for module \"" + mModuleName + "\".");
    }

    // Load the metadata file.
    JSONDocument mMetadataDocument(mMetadataPath + ".json", false);

    // Load the module and component type metadata.  This needs to be done before the module is created.
    cDescription = mMetadataDocument.getString("description");
    cLongName = mMetadataDocument.hasMember("longName") ? mMetadataDocument.getString("longName") : mModuleName;

    if (mMetadataDocument.hasMember("categories")) {
      JSONObject mCategoriesObject = mMetadataDocument.getObject("categories");
      for (JSONThing mCategoryThing : mCategoriesObject) {
        std::string mCategoryDescription = mCategoryThing.getValueAsString();
        std::string mCategoryName = mCategoryThing.getName();
        cCategoryDescriptions[mCategoryName] = mCategoryDescription;
      }
    }

    JSONObject mComponentsObject = mMetadataDocument.getObject("components");
    for (JSONThing mComponentThing : mComponentsObject) {
      JSONObject mComponentObject = mComponentThing.getValue();
      std::string mComponentName = mComponentThing.getName();
      cComponentTypes[mComponentName] = std::make_unique<ComponentTypeMetadata>(mComponentObject);
    }
  }

  void ModuleMetadata::scopeCategories(IComponentDefiner& definer, IPropertyManager& properties, Module& module, std::function<void()> removeFunction) {

    // Add a module heading with remove icon to unload the module.
    properties.addRemover(cLongName, removeFunction);

    // Scope component types by category.
    properties.pushIndent();
    for (const std::pair<const std::string, std::string>& mCategory : cCategoryDescriptions) {
      Options mNamelessHint;
      mNamelessHint.addOption("name", "");
      mNamelessHint.addOption("description", mCategory.second);
      definer.scope(mCategory.first, mCategory.first, [this, &module, mCategory](IComponentDefiner& definer) {
        for (const std::pair<const std::string, std::unique_ptr<ComponentTypeMetadata>>& mComponentType : cComponentTypes) {
          mComponentType.second->scope(definer, module, mComponentType.first, mCategory.first);
        }
      }, nullptr, mNamelessHint);
    }

    // Scope component types without a category.
    for (const std::pair<const std::string, std::unique_ptr<ComponentTypeMetadata>>& mComponentType : cComponentTypes) {
      mComponentType.second->scope(definer, module, mComponentType.first, "");
    }
    properties.popIndent();
  }
}
 