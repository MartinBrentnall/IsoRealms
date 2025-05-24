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

#include <memory>
#include <vector>
#include <string>

#include "IsoRealms/IProject.h"

namespace IsoRealms {
  class File {
    public:
    File(IProject& project, std::function<void()> changeCallback = nullptr);
    std::string getPath() const;
    void load(const std::string& name, JSONObject object);
    void save(const std::string& name, JSONObject object) const;

    /*******************************\
     * Interface for PropertyAsset *
    \*******************************/
    std::string getID() const;
    bool renderAssetIcon() const;
    bool hasConfiguration() const;
    bool isDefaultConfigured() const;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties();
    IApplication& getApplication() const;
    std::vector<std::string> getAvailableProviders() const;
    bool renderProviderIcon(const std::string& id) const;
    void setID(const std::string& id);
    
    private:
    static const std::string LOCATION_PREFIX_USER;
    static const std::string LOCATION_PREFIX_PROGRAM;
    
    static const std::string JSON_PATH;
    static const std::string JSON_USER;

    IProject& cProject;
    std::function<void()> cChangeCallback;
    bool cUser;
    std::string cPath;
    
    static void getFilesAt(const std::string& path, const std::string& prefix, std::vector<std::string>& list);
  };
}
