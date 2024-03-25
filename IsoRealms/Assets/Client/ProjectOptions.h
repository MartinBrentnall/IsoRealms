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

#include <functional>

#include "IsoRealms/Assets/Type/IProjectOptions.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms {
  class ProjectOptions : public IAssetUser<IProjectOptions> {
    private:
    IProject* cProject;
    IProjectOptions* cProjectOptions;

    ProjectOptions(ProjectOptions const& projectOptions) = delete;
    ProjectOptions& operator=(ProjectOptions const& projectOptions) = delete;

    public:
    ProjectOptions(IProject* project);

    void init(DOMNode& node);
    void save(DOMNodeWriter* node, const std::string& tag) const;

    void set(DOMNode& node);
    IProjectOptions* operator->() const {
      return cProjectOptions;
    }

    IProjectOptions* operator*() const {
      return cProjectOptions;
    }

    /******************************************\
     * Implements IAssetUser<IProjectOptions> *
    \******************************************/
    void relinquish(IProjectOptions* asset) override;

    virtual ~ProjectOptions();
  };
}
