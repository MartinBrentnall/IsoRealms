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
#pragma once

#include <memory>

#include "IAsset.h"
#include "IEditableScreen.h"

namespace IsoRealms {
  class IDialogManager;
  class IResourceData;
  class Project;

  /**
   * Interface for Editable assets.
   */
  class IEditable : public IAsset {
    public:

    /**
     * Load complex editor content that cannot be represented as simple properties.
     */
    virtual void load(IResourceData& resourceData, JSONObject object) {}

    /**
     * Persist complex editor content that cannot be represented as simple properties.
     */
    virtual void save(IResourceData& resourceData, JSONObject object) const {}

    /**
     * Create a screen to edit this editable.
     * 
     * @return Screen to edit this editable.
     */
    virtual IEditableScreen* createEditableScreen(Project* project, IDialogManager& dialogManager) = 0;
    
    virtual ~IEditable() {}
  };
}
