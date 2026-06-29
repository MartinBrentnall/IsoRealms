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

#include "Model.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/PropertyNativeFloat.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  Model::Model(IComponentData& owner) :
            Resource<Model, IModel, IComponentData>(owner) {
  }

  std::unique_ptr<ModelInstance> Model::createInstance() {
    std::unique_ptr<ModelInstance> mInstance = std::make_unique<ModelInstance>(*this, cResource->createModel());
    cInstances.emplace_back(mInstance.get());
    return mInstance;
  }

  bool Model::renderIcon() const {
    applyTransformation();
    return cResource->renderResourceIcon();
  }

  bool Model::renderPreview() const {
    applyTransformation();
    return cResource->renderPreview();
  }

  void Model::notifyDestruction(ModelInstance* instance) {
    Utils::removeElement(cInstances, instance);
  }
  
  void Model::applyTransformation() const {
    glRotatef(cDefPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cDefYaw, 0.0f, 0.0f, 1.0f);
    glTranslatef(cDefOffsetX, cDefOffsetY, cDefOffsetZ);
    glScalef(cDefScaleX, cDefScaleY, cDefScaleZ);
  }

  bool Model::hasClientConfiguration() const {
    return true;
  }

  bool Model::isDefaultConfiguration() const {
    return cDefOffsetX == 0.0f && cDefOffsetY == 0.0f && cDefOffsetZ == 0.0f && cDefPitch == 0.0f && cDefScaleX == 1.0f && cDefScaleY == 1.0f && cDefScaleZ == 1.0f && cDefYaw == 0.0f;
  }

  void Model::defineWrapper(IComponentDefiner& definer) {
    definer.scope("", "", [this, &definer]() {
      definer.propertyFloat("offsetX", cDefOffsetX);
      definer.propertyFloat("offsetY", cDefOffsetY);
      definer.propertyFloat("offsetZ", cDefOffsetZ);
      definer.propertyFloat("scaleX",  cDefScaleX, 1.0f);
      definer.propertyFloat("scaleY",  cDefScaleY, 1.0f);
      definer.propertyFloat("scaleZ",  cDefScaleZ, 1.0f);
      definer.propertyFloat("yaw",     cDefYaw);
      definer.propertyFloat("pitch",   cDefPitch);
    }, nullptr, IComponentDefiner::resourceMetadataHint("", "Model") + IComponentDefiner::HINT_INLINE);

    definer.onInitialised([this]() {
      for (ModelInstance* mInstance : cInstances) {
        mInstance->set(cResource->createModel());
      }
    });
  }
}
