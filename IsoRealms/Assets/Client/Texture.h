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

#include "IsoRealms/Assets/Type/ITexture.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/IStateListener.h"

#include "Asset.h"

namespace IsoRealms {
  class Texture : public Asset<Texture, ITexture, IResourceData>,
                  public IStateListener<ITexture*> {
    public:
    Texture(IResourceData& owner, std::function<void()> listener = nullptr);

    void coord(float x, float y) const;

    /*********************************************\
     * Implements Asset<ITexture, IResourceData> *
    \*********************************************/
    ITexture* createLiteralAsset(IResourceData& owner);
    ITexture* getAsset(IResourceData& owner, JSONObject object);
    ITexture* getAsset(IResourceData& owner, const std::string& id);
    bool hasClientConfiguration() const;
    bool isDefaultConfiguration() const;

    void loadClientConfiguration(JSONObject object) override;
    void saveClientConfiguration(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getClientProperties() override;

    /****************************************\
     * Implements IStateListener<ITexture*> *
    \****************************************/
    void stateChanged(ITexture* asset) override;

    private:
    static const std::string JSON_ANGLE;
    static const std::string JSON_SCALE_X;
    static const std::string JSON_SCALE_Y;
    
    std::function<void()> cListener;
    float cDefScaleX;
    float cDefScaleY;
    float cDefAngle;
  };
}
