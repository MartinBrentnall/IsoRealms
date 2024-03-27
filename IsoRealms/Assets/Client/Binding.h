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

#include "IsoRealms/Assets/IBindingRegistry.h"
#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms {
  class Binding : public IAssetUser<IBinding> {
    public:
    Binding(IProject* project, IBindingRegistry* registry);

    void init(DOMNode& node);
    void save(DOMNodeWriter* node, bool local, const std::string& attribute) const;

    void set(DOMNode& node);
    IBinding* operator->() const {
      return cDefBinding;
    }

    IBinding* operator*() const {
      return cDefBinding;
    }

    /***********************************\
     * Implements IAssetUser<IBinding> *
    \***********************************/
    void relinquish(IBinding* asset) override;

    virtual ~Binding();

    private:
    IProject* cProject;
    IBindingRegistry* cDefRegistry;
    IBinding* cDefBinding;

    Binding(Binding const& asset) = delete;
    Binding& operator=(Binding const& asset) = delete;
  };
}
