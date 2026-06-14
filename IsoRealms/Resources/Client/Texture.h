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

#include "IsoRealms/Resources/Type/ITexture.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/IStateListener.h"

#include "Resource.h"

namespace IsoRealms {
  class Texture : public Resource<Texture, ITexture, IComponentData>,
                  public IStateListener {
    public:
    Texture(IComponentData& owner, std::function<void()> listener = nullptr);

    void coord(float x, float y) const;

    /*************************************************\
     * Implements Resource<ITexture, IComponentData> *
    \*************************************************/
    bool hasClientConfiguration() const;
    bool isDefaultConfiguration() const;

    void loadClientConfiguration(JSONObject object) override;
    void saveClientConfiguration(JSONObject object) const override;
    void getClientProperties(IComponentDefiner& definer) override;

    /***********************************************\
     * Implements ITreeSelectorObject via Resource *
    \***********************************************/
    const Metadata& getPropertyMetadata() const override;

    /****************************************\
     * Implements IStateListener<ITexture*> *
    \****************************************/
    void stateChanged() override;

    private:
    inline static const std::string JSON_ANGLE   = "angle";
    inline static const std::string JSON_SCALE_X = "scaleX";
    inline static const std::string JSON_SCALE_Y = "scaleY";
    
    std::function<void()> cListener;
    float cDefScaleX;
    float cDefScaleY;
    float cDefAngle;
  };
}
