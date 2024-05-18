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
#include "Model.h"

namespace IsoRealms {
  const std::string Model::JSON_OFFSET_X = "offsetX";
  const std::string Model::JSON_OFFSET_Y = "offsetY";
  const std::string Model::JSON_OFFSET_Z = "offsetZ";
  const std::string Model::JSON_PITCH    = "pitch";
  const std::string Model::JSON_SCALE_X  = "scaleX";
  const std::string Model::JSON_SCALE_Y  = "scaleY";
  const std::string Model::JSON_SCALE_Z  = "scaleZ";
  const std::string Model::JSON_YAW      = "yaw";

  Model::Model(IProject* project) :
            cProject(project),
            cModel(cProject->createLiteral3DModel(this)) {
  }

  void Model::init(JSONObject object, const std::string& member) {
    cProject->init([this, object, member](IAssets* assets) {
      set(object, member);
    });
  }

  void Model::set(JSONObject object, const std::string& member) {
    JSONObject mAssetObject = object.getObject(member);
    cProject->release(this, cModel);
    cDefOffsetX = mAssetObject.getFloat(JSON_OFFSET_X, 0.0f);
    cDefOffsetY = mAssetObject.getFloat(JSON_OFFSET_Y, 0.0f);
    cDefOffsetZ = mAssetObject.getFloat(JSON_OFFSET_Z, 0.0f);
    cDefPitch   = mAssetObject.getFloat(JSON_PITCH,    0.0f);
    cDefScaleX  = mAssetObject.getFloat(JSON_SCALE_X,  1.0f);
    cDefScaleY  = mAssetObject.getFloat(JSON_SCALE_Y,  1.0f);
    cDefScaleZ  = mAssetObject.getFloat(JSON_SCALE_Z,  1.0f);
    cDefYaw     = mAssetObject.getFloat(JSON_YAW,      0.0f);
    cModel = cProject->getModelType(this, mAssetObject);
    for (ModelInstance* mInstance : cInstances) {
      mInstance->set(cModel->createModel());
    }
  }

  void Model::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    mAssetObject.addFloat(JSON_OFFSET_X, cDefOffsetX);
    mAssetObject.addFloat(JSON_OFFSET_Y, cDefOffsetY);
    mAssetObject.addFloat(JSON_OFFSET_Z, cDefOffsetZ);
    mAssetObject.addFloat(JSON_PITCH,    cDefPitch);
    mAssetObject.addFloat(JSON_SCALE_X,  cDefScaleX, 1.0f);
    mAssetObject.addFloat(JSON_SCALE_Y,  cDefScaleY, 1.0f);
    mAssetObject.addFloat(JSON_SCALE_Z,  cDefScaleZ, 1.0f);
    mAssetObject.addFloat(JSON_YAW,      cDefYaw);
    cProject->save(mAssetObject, cModel);
  }

  std::unique_ptr<ModelInstance> Model::createInstance() {
    std::unique_ptr<ModelInstance> mInstance = std::make_unique<ModelInstance>(this, cModel->createModel());
    cInstances.emplace_back(mInstance.get());
    return mInstance;
  }

  bool Model::renderIcon() const {
    applyTransformation();
    return cModel->renderAssetIcon();
  }

  bool Model::renderPreview() const {
    applyTransformation();
    return cModel->renderPreview();
  }

  void Model::applyTransformation() const {
    glRotatef(cDefPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cDefYaw, 0.0f, 0.0f, 1.0f);
    glTranslatef(cDefOffsetX, cDefOffsetY, cDefOffsetZ);
    glScalef(cDefScaleX, cDefScaleY, cDefScaleZ);
  }

  void Model::relinquish(I3DModelType* asset) {
    if (cModel == asset) {
      cModel = cProject->createLiteral3DModel(this);
      for (ModelInstance* mInstance : cInstances) {
        mInstance->set(cModel->createModel());
      }
    }
  }

  void Model::notifyDestruction(ModelInstance* instance) {
    Utils::removeElement(cInstances, instance);
  }

  Model::~Model() {
    if (cModel != nullptr) {
      cProject->release(this, cModel);
      // TODO: Update instances with literal???  Actually... maybe the release will handle it already, and if it's being destructed then there should be no instances left?
    }
  }
}
