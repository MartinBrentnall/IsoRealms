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

namespace IsoRealms {
  class File;

  class IResourceData {
    public:
    virtual std::string getPath(const std::string& file, bool user) const = 0;
    virtual void makeUserDataDirectory() = 0;
    virtual bool isIncluded() const = 0;
    virtual bool isReadOnly() const = 0;
    virtual void setOwner(File* owner) = 0;
    virtual IProject& getProject() = 0;
    virtual IProject& getAssetManager() = 0;
    virtual IActionClient& getDummyActionClient() = 0;
  };
}
