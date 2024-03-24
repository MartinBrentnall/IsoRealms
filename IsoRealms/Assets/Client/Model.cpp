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
  const std::string Model::ATTRIBUTE_OFFSET_X = "offsetX";
  const std::string Model::ATTRIBUTE_OFFSET_Y = "offsetY";
  const std::string Model::ATTRIBUTE_OFFSET_Z = "offsetZ";
  const std::string Model::ATTRIBUTE_PITCH    = "pitch";
  const std::string Model::ATTRIBUTE_SCALE_X  = "scaleX";
  const std::string Model::ATTRIBUTE_SCALE_Y  = "scaleY";
  const std::string Model::ATTRIBUTE_SCALE_Z  = "scaleZ";
  const std::string Model::ATTRIBUTE_YAW      = "yaw";
    
  Model::Model(IProject* project) :
            cProject(project),
            cModel(cProject->createLiteral3DModel(this)) {
  }

  void Model::init(DOMNode& node) {
    cDefOffsetX = node.getFloatAttribute(ATTRIBUTE_OFFSET_X, 0.0f);
    cDefOffsetY = node.getFloatAttribute(ATTRIBUTE_OFFSET_Y, 0.0f);
    cDefOffsetZ = node.getFloatAttribute(ATTRIBUTE_OFFSET_Z, 0.0f);
    cDefPitch   = node.getFloatAttribute(ATTRIBUTE_PITCH,    0.0f);
    cDefScaleX  = node.getFloatAttribute(ATTRIBUTE_SCALE_X,  1.0f);
    cDefScaleY  = node.getFloatAttribute(ATTRIBUTE_SCALE_Y,  1.0f);
    cDefScaleZ  = node.getFloatAttribute(ATTRIBUTE_SCALE_Z,  1.0f);
    cDefYaw     = node.getFloatAttribute(ATTRIBUTE_YAW,      0.0f);
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Model::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mModelNode = node->addBranch(tag);
    cProject->save(&mModelNode, cModel);
    mModelNode.addAttribute(ATTRIBUTE_OFFSET_X, cDefOffsetX);
    mModelNode.addAttribute(ATTRIBUTE_OFFSET_Y, cDefOffsetY);
    mModelNode.addAttribute(ATTRIBUTE_OFFSET_Z, cDefOffsetZ);
//    mModelNode.addAttribute(ATTRIBUTE_MODEL,    cProject->getID(cModel));
    mModelNode.addAttribute(ATTRIBUTE_PITCH,    cDefPitch);
    mModelNode.addAttribute(ATTRIBUTE_SCALE_X,  cDefScaleX);
    mModelNode.addAttribute(ATTRIBUTE_SCALE_Y,  cDefScaleY);
    mModelNode.addAttribute(ATTRIBUTE_SCALE_Z,  cDefScaleZ);
    mModelNode.addAttribute(ATTRIBUTE_YAW,      cDefYaw);
  }

  std::unique_ptr<ModelInstance> Model::createInstance() {
    std::unique_ptr<ModelInstance> mInstance = std::make_unique<ModelInstance>(this, cModel->createModel());
    cInstances.emplace_back(mInstance.get());
    return mInstance;
  }

  bool Model::renderIcon() const {
    glScalef(cDefScaleX, cDefScaleY, cDefScaleZ);
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

  std::string Model::get() const {
    return cProject->getID(cModel);
  }

  void Model::set(DOMNode& node) {
    cProject->release(this, cModel);
    cModel = cProject->getModelType(this, node);
    for (ModelInstance* mInstance : cInstances) {
      mInstance->set(cModel->createModel());
    }
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
