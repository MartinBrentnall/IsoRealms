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

#include "IsoRealms/Resources/Type/IModel.h"
#include "IsoRealms/IComponentData.h"

#include "Resource.h"
#include "ModelInstance.h"

namespace IsoRealms {
  class Model : public Resource<Model, IModel, IComponentData> {
    public:
    Model(IComponentData& owner);

    std::unique_ptr<ModelInstance> createInstance();
    bool renderIcon() const;
    bool renderPreview() const;
    void notifyDestruction(ModelInstance* instance);
    void applyTransformation() const;

    /***********************************************\
     * Implements Resource<IModel, IComponentData> *
    \***********************************************/
    bool hasClientConfiguration() const;
    bool isDefaultConfiguration() const;

    void defineWrapper(IComponentDefiner& definer) override;

    private:
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
