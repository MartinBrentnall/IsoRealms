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

#include "IsoRealms/Assets/Type/IFloat.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"


namespace IsoRealms {
  class Float : public IAssetUser<IFloat>,
                      public IStateListener<IFloat*> {
    private:
    IProject* cProject;
    float cDefaultValue;
    IFloat* cFloat;
    std::function<void(float)> cListener;

    Float(Float const& other) = delete;
    Float& operator=(Float const& other) = delete;

    public:
    Float(IProject* project, float defaultValue = 0.0f, std::function<void(float)> listener = nullptr);

    void init(DOMNode& node, const std::string& tag);
    void set(DOMNode& node, const std::string& tag);
    void save(DOMNodeWriter* node, const std::string& tag) const;

    IFloat* operator->() const {
      return cFloat;
    }

    IFloat* operator*() const {
      return cFloat;
    }

    /*********************************\
     * Implements IAssetUser<IFloat> *
    \*********************************/
    void relinquish(IFloat* asset) override;

    /**************************************\
     * Implements IStateListener<IFloat*> *
    \**************************************/
    void stateChanged(IFloat* asset) override;

    virtual ~Float();
  };
}
