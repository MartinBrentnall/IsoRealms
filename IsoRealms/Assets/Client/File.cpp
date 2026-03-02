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
#include "File.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/System.h"

namespace IsoRealms {
  File::File(Project& project, std::function<void()> changeCallback) :
            cProject(project),
            cChangeCallback(changeCallback) {
  }
  
  void File::setPath(const std::string& path, bool user) {
    cPath = path;
    cUser = user;
  }

  std::string File::getPath() const {
    return System::getPath(cPath, cUser);
  }

  std::string File::getRelativePath() const {
    return cPath;
  }

  bool File::isUser() const {
    return cUser;
  }

  bool File::isSet() const {
    return !cPath.empty();
  }
  
  void File::load(const std::string& name, JSONObject object) {
    JSONObject mFileObject = object.getObject(name);
    cPath = mFileObject.getString(JSON_PATH);
    cUser = mFileObject.getBoolean(JSON_USER);
  }
  
  void File::save(const std::string& name, JSONObject object) const {
    JSONObject mFileObject = object.addObject(name);
    mFileObject.addString(JSON_PATH, cPath);
    mFileObject.addBoolean(JSON_USER, cUser);
  }
  
  AssetInfo File::getAssetInfo() const {
    std::string id = std::string(cUser ? "User" : "Program") + "/" + cPath;
    for (const AssetInfo& e : getAvailableProviders()) {
      if (e.cID == id) return e;
    }
    return AssetInfo{id, ""};
  }
  
  bool File::renderAssetIcon() const {
    return false;
  }
  
  bool File::hasConfiguration() const {
    return false;
  }

  bool File::isDefaultConfigured() const {
    return true;
  }

  void File::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  Application& File::getApplication() const {
    return cProject.getApplication();
  }
  
  std::vector<AssetInfo> File::getAvailableProviders() const {
    std::vector<std::string> mFiles;
    getFilesAt(System::getPath("./", false), LOCATION_PREFIX_PROGRAM, mFiles);
    getFilesAt(System::getPath("", true), LOCATION_PREFIX_USER, mFiles);
    std::sort(mFiles.begin(), mFiles.end());
    std::vector<AssetInfo> result;
    for (const std::string& mFile : mFiles) {
      result.emplace_back(AssetInfo{mFile, mFile});
    }
    return result;
  }
  
  bool File::renderProviderIcon(const std::string& id) const {
    return false;
  }
  
  void File::setID(const std::string& id) {
    if (id.rfind(LOCATION_PREFIX_USER, 0) == 0) {
      cUser = true;
      cPath = id.substr(LOCATION_PREFIX_USER.length() + 1);
    } else if (id.rfind(LOCATION_PREFIX_PROGRAM, 0) == 0) {
      cUser = false;
      cPath = id.substr(LOCATION_PREFIX_PROGRAM.length() + 1);
    } else {
//    TODO  throw ArgumentException("Unsupported File ID \"" + id + "\"");
    }
    cChangeCallback();
  }
  
  const std::string File::LOCATION_PREFIX_USER    = "User";
  const std::string File::LOCATION_PREFIX_PROGRAM = "Program";

  const std::string File::JSON_PATH = "path";
  const std::string File::JSON_USER = "user";

  void File::getFilesAt(const std::string& path, const std::string& prefix, std::vector<std::string>& list) {
    std::cout << "Getting files at path: \"" << path << "\"..." << std::endl;
    for (const std::filesystem::directory_entry& mDirEntry : std::filesystem::recursive_directory_iterator{path}) {
      if (!mDirEntry.is_directory()) {
        list.emplace_back(prefix + "/" + mDirEntry.path().string().substr(path.length()));
        std::cout << "  Got path: \"" << list.back() << "\"..." << std::endl;
      }
    }
  }  
}
