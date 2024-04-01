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

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Persistence/DOMNode.h"

namespace IsoRealms {
  class DOMNodeWriter;
  class IBinding;

  class IBindingRegistry {
    public:
    virtual IBinding* getBinding(const std::string& id) = 0;
    virtual void saveBinding(DOMNodeWriter* node, const IBinding* binding) const = 0;
    virtual void releaseBinding(const IBinding* asset) = 0;
  };
}
