/*
 * Copyright 2016 Martin Brentnall
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

ProjectOptions::ProjectOptions(DOMNodeWrapper* node, std::map<std::string, std::string> referenceOptions) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ModuleOptions") {
      std::string mModuleName = mNode->getAttribute("name");
      cProjectOptions[mModuleName] = new ModuleOptions(mNode, referenceOptions);
    }
  }
}

IModuleOptions* ProjectOptions::getModuleOptions(const std::string& moduleName) {
  std::map<std::string, IModuleOptions*>::iterator mModuleOptions = cProjectOptions.find(moduleName);
  if (mModuleOptions != cProjectOptions.end()) {
    return mModuleOptions->second;
  }
  return nullptr;
}

ProjectOptions::ModuleOptions::ModuleOptions(DOMNodeWrapper* node, std::map<std::string, std::string> referenceOptions) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Option") {
      std::string mName  = mNode->getAttribute("name");
      std::string mValue = mNode->getAttribute("value");
      if (mValue == "") {
        std::string mReference = mNode->getAttribute("ref");
        mValue = referenceOptions[mReference];
      }
      if (mValue == "") {
        std::cout << "Missing option value for option \"" << mName << "\"" << std::endl;
        exit(1);
      }
      cModuleOptions[mName] = mValue;
    }
  }  
}

std::string ProjectOptions::ModuleOptions::getOption(const std::string& option) {
  std::map<std::string, std::string>::iterator mOption = cModuleOptions.find(option);
  if (mOption != cModuleOptions.end()) {
    return mOption->second;
  }
  // TODO: Throw
  return "";
}
