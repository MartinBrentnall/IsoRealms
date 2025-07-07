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

#include <functional>

#include "IsoRealms/Assets/Type/IFloat.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/IStateListener.h"

#include "Asset.h"

namespace IsoRealms {
  class Float : public Asset<Float, IFloat, IResourceData>,
                public IStateListener<IFloat*> {
    public:
    Float(IResourceData& owner, float defaultValue = 0.0f, std::function<void(float)> listener = nullptr);

    /*******************************************\
     * Implements Asset<IFloat, IResourceData> *
    \*******************************************/
    IFloat* createLiteralAsset(IResourceData& owner);
    IFloat* getAsset(IResourceData& owner, JSONObject object);
    IFloat* getAsset(IResourceData& owner, const std::string& id);
    bool isDefaultConfiguration() const;

    /**************************************\
     * Implements IStateListener<IFloat*> *
    \**************************************/
    void stateChanged(IFloat* asset) override;

    private:
    float cDefaultValue;
    std::function<void(float)> cListener;
  };
}
