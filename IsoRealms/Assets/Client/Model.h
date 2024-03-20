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

#include "IsoRealms/Assets/Type/I3DModelType.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/Utils.h"

#include "ModelInstance.h"

namespace IsoRealms {
  class Model : public IAssetUser<I3DModelType> {
    private:
    static const std::string ATTRIBUTE_OFFSET_X;
    static const std::string ATTRIBUTE_OFFSET_Y;
    static const std::string ATTRIBUTE_OFFSET_Z;
    static const std::string ATTRIBUTE_PITCH;
    static const std::string ATTRIBUTE_SCALE_X;
    static const std::string ATTRIBUTE_SCALE_Y;
    static const std::string ATTRIBUTE_SCALE_Z;
    static const std::string ATTRIBUTE_YAW;
    
    IProject* cProject;
    I3DModelType* cModel;
    float cDefOffsetX;
    float cDefOffsetY;
    float cDefOffsetZ;
    float cDefYaw;
    float cDefPitch;
    float cDefScaleX;
    float cDefScaleY;
    float cDefScaleZ;
    std::vector<ModelInstance*> cInstances;

    Model(Model const& model) = delete;
    Model& operator=(Model const& model) = delete;
    
    public:
    Model(IProject* project);

    void init(DOMNode& node);
    void save(DOMNodeWriter* node, const std::string& tag) const;

    std::string get() const;
    void set(DOMNode& node);
    std::unique_ptr<ModelInstance> createInstance();
    bool renderIcon() const;
    bool renderPreview() const;
    void applyTransformation() const;

    /***************************************\
     * Implements IAssetUser<I3DModelType> *
    \***************************************/
    void relinquish(I3DModelType* asset) override;

    void notifyDestruction(ModelInstance* instance);

    virtual ~Model();
  };
}
