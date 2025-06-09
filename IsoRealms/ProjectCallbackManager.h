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
#include <vector>

#include "ICallbackHandle.h"

namespace IsoRealms {
  class IProject;

  class ProjectCallbackManager {
    public:
    ProjectCallbackManager(IProject& project);
    void reset(std::function<void()> resetter);
    void updateRuntime(std::function<void(unsigned int)> dynamic);
    void updateEditing(std::function<void(unsigned int)> dynamic);
    ~ProjectCallbackManager();

    private:
    IProject& cProject;
    std::vector<ICallbackHandle*> cCallbackHandles;
  };
}
