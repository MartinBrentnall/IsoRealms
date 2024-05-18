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

#include "IsoRealms/Assets/Type/IEditable.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms {
  class Editable : public IAssetUser<IEditable> {
    private:
    IProject* cProject;
    IEditable* cEditable;

    Editable(Editable const& editable) = delete;
    Editable& operator=(Editable const& editable) = delete;

    public:
    Editable(IProject* project);

    void init(JSONObject object, const std::string& member);
    void set(JSONObject object, const std::string& member);
    void save(JSONObject object, const std::string& name) const;

    IEditable* operator->() const {
      return cEditable;
    }

    IEditable* operator*() const {
      return cEditable;
    }

    /************************************\
     * Implements IAssetUser<IEditable> *
    \************************************/
    void relinquish(IEditable* asset) override;

    virtual ~Editable();
  };
}
