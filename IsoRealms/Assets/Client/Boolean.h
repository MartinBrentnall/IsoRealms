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

#include "IsoRealms/Assets/Type/IBoolean.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"


namespace IsoRealms {
  class Boolean : public IAssetUser<IBoolean>,
                  public IStateListener<IBoolean*> {
    public:
    Boolean(IProject* project, bool defaultValue, std::function<void(bool)> listener = nullptr);

    void init(DOMNode& node);
    void save(DOMNodeWriter* node, const std::string& tag) const;

    void set(DOMNode& node);
    IBoolean* operator->() const {
      return cBoolean;
    }

    IBoolean* operator*() const {
      return cBoolean;
    }

    /***********************************\
     * Implements IAssetUser<IBoolean> *
    \***********************************/
    void relinquish(IBoolean* asset) override;

    /****************************************\
     * Implements IStateListener<IBoolean*> *
    \****************************************/
    void stateChanged(IBoolean* asset) override;

    virtual ~Boolean();

    private:
    IProject* cProject;
    bool cDefaultValue;
    IBoolean* cBoolean;
    std::function<void(bool)> cListener;

    Boolean(Boolean const& boolean) = delete;
    Boolean& operator=(Boolean const& boolean) = delete;
  };
}
