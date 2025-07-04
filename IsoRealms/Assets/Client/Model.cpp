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

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/PropertyNativeFloat.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  Model::Model(IResourceData& owner) :
            Asset<Model, IModel, IResourceData>(owner, owner.getAssetManager().createLiteralModel(this, owner)) {
  }

  std::unique_ptr<ModelInstance> Model::createInstance() {
    std::unique_ptr<ModelInstance> mInstance = std::make_unique<ModelInstance>(*this, cAsset->createModel());
    cInstances.emplace_back(mInstance.get());
    return mInstance;
  }

  bool Model::renderIcon() const {
    applyTransformation();
    return cAsset->renderAssetIcon();
  }

  bool Model::renderPreview() const {
    applyTransformation();
    return cAsset->renderPreview();
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

  IModel* Model::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralModel(this, owner);
  }
  
  IModel* Model::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getModel(this, object, owner);
  }
  
  IModel* Model::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getModel(this, id, owner);
  }
  
  bool Model::hasClientConfiguration() const {
    return true;
  }

  bool Model::isDefaultConfiguration() const {
    return cDefOffsetX == 0.0f && cDefOffsetY == 0.0f && cDefOffsetZ == 0.0f && cDefPitch == 0.0f && cDefScaleX == 1.0f && cDefScaleY == 1.0f && cDefScaleZ == 1.0f && cDefYaw == 0.0f;
  }

  void Model::loadClientConfiguration(JSONObject object) {
    cDefOffsetX = object.getFloat(JSON_OFFSET_X, 0.0f);
    cDefOffsetY = object.getFloat(JSON_OFFSET_Y, 0.0f);
    cDefOffsetZ = object.getFloat(JSON_OFFSET_Z, 0.0f);
    cDefPitch   = object.getFloat(JSON_PITCH,    0.0f);
    cDefScaleX  = object.getFloat(JSON_SCALE_X,  1.0f);
    cDefScaleY  = object.getFloat(JSON_SCALE_Y,  1.0f);
    cDefScaleZ  = object.getFloat(JSON_SCALE_Z,  1.0f);
    cDefYaw     = object.getFloat(JSON_YAW,      0.0f);
    for (ModelInstance* mInstance : cInstances) {
      mInstance->set(cAsset->createModel());
    }
  }

  void Model::saveClientConfiguration(JSONObject object) const {
    object.addFloat(JSON_OFFSET_X, cDefOffsetX);
    object.addFloat(JSON_OFFSET_Y, cDefOffsetY);
    object.addFloat(JSON_OFFSET_Z, cDefOffsetZ);
    object.addFloat(JSON_PITCH,    cDefPitch);
    object.addFloat(JSON_SCALE_X,  cDefScaleX, 1.0f);
    object.addFloat(JSON_SCALE_Y,  cDefScaleY, 1.0f);
    object.addFloat(JSON_SCALE_Z,  cDefScaleZ, 1.0f);
    object.addFloat(JSON_YAW,      cDefYaw);
  }

  std::vector<std::unique_ptr<IProperty>> Model::getClientProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Offset X", "TODO", [this]() {return cDefOffsetX;}, [this](float value) {cDefOffsetX = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Offset Y", "TODO", [this]() {return cDefOffsetY;}, [this](float value) {cDefOffsetY = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Offset Z", "TODO", [this]() {return cDefOffsetZ;}, [this](float value) {cDefOffsetZ = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Scale X",  "TODO", [this]() {return cDefScaleX;},  [this](float value) {cDefScaleX  = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Scale Y",  "TODO", [this]() {return cDefScaleY;},  [this](float value) {cDefScaleY  = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Scale Z",  "TODO", [this]() {return cDefScaleZ;},  [this](float value) {cDefScaleZ  = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Angle",    "TODO", [this]() {return cDefYaw;},     [this](float value) {cDefYaw     = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Tilt",     "TODO", [this]() {return cDefPitch;},   [this](float value) {cDefPitch   = value; return true;}));
    return mProperties;
  }
  
  const std::string Model::JSON_OFFSET_X = "offsetX";
  const std::string Model::JSON_OFFSET_Y = "offsetY";
  const std::string Model::JSON_OFFSET_Z = "offsetZ";
  const std::string Model::JSON_PITCH    = "pitch";
  const std::string Model::JSON_SCALE_X  = "scaleX";
  const std::string Model::JSON_SCALE_Y  = "scaleY";
  const std::string Model::JSON_SCALE_Z  = "scaleZ";
  const std::string Model::JSON_YAW      = "yaw";
}
