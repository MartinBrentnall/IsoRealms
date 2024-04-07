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
#include "ProjectOptions.h"

namespace IsoRealms::Basics {
  const std::string ProjectOptions::TAG_OPTION = "Option";
  const std::string ProjectOptions::TAG_VALUE  = "Value";

  const std::string ProjectOptions::ATTRIBUTE_ID = "id";

  ProjectOptions::ProjectOptions(IProject* project, Basics* basics) {
  }
  
  ProjectOptions::ProjectOptions(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            ProjectOptions(project, basics) {
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == TAG_OPTION) {
        std::string mName = mNode.getAttribute(ATTRIBUTE_ID);
        cDefOptions.emplace(std::piecewise_construct, std::forward_as_tuple(mName), std::forward_as_tuple(project));
        cDefOptions.find(mName)->second.init(mNode, TAG_VALUE);
      } else {
        throw ParseException("Unknown tag for Basics/ProjectOptions: " + mChildName);
      }
    }
  }

  void ProjectOptions::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Project Options");
  }
  
  void ProjectOptions::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void ProjectOptions::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::pair<const std::string, String>& mOption : cDefOptions) {
      DOMNodeWriter mOptionNode = node->addBranch(TAG_OPTION);
      mOptionNode.addAttribute(mOption.first, ATTRIBUTE_ID);
      mOption.second.save(&mOptionNode, TAG_VALUE);
    }
  }

  void ProjectOptions::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ProjectOptions::renderIcon() {
    return false;
  }

  std::vector<IProperty*> ProjectOptions::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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
}
