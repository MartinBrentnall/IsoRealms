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

#include "IsoRealms/Assets/Type/IModel.h"
#include "IsoRealms/IResourceData.h"

#include "Asset.h"
#include "ModelInstance.h"

namespace IsoRealms {
  class Model : public Asset<Model, IModel, IResourceData> {
    public:
    Model(IResourceData& owner);

    std::unique_ptr<ModelInstance> createInstance();
    bool renderIcon() const;
    bool renderPreview() const;
    void notifyDestruction(ModelInstance* instance);
    void applyTransformation() const;

    /*********************************************\
     * Implements Asset<IInteger, IResourceData> *
    \*********************************************/
    bool hasClientConfiguration() const;
    bool isDefaultConfiguration() const;

    void loadClientConfiguration(JSONObject object) override;
    void saveClientConfiguration(JSONObject object) const override;
    void getClientProperties(PropertyMaker& owner) override;

    private:
    static const std::string JSON_OFFSET_X;
    static const std::string JSON_OFFSET_Y;
    static const std::string JSON_OFFSET_Z;
    static const std::string JSON_PITCH;
    static const std::string JSON_SCALE_X;
    static const std::string JSON_SCALE_Y;
    static const std::string JSON_SCALE_Z;
    static const std::string JSON_YAW;

    float cDefOffsetX;
    float cDefOffsetY;
    float cDefOffsetZ;
    float cDefYaw;
    float cDefPitch;
    float cDefScaleX;
    float cDefScaleY;
    float cDefScaleZ;
    std::vector<ModelInstance*> cInstances;
  };
}
