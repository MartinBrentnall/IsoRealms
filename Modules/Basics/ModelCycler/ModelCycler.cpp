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
#include "ModelCycler.h"

namespace IsoRealms::Basics {
  const std::string ModelCycler::JSON_MODEL  = "model";
  const std::string ModelCycler::JSON_MODELS = "models";

  ModelCycler::ModelCycler(IProject& project, Basics& basics, IResourceData& data) :
            cProjectCallbackManager(project),
            cRuntimeCycleIndex(0),
            cLuaBinding(project, this),
            cEditingIconCycle(0) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeCycleIndex = 0; 
    });
    
    cProjectCallbackManager.updateEditing([this](unsigned int milliseconds) {
      cEditingIconCycle += milliseconds;
      if (cEditingIconCycle >= cOffsetModels.size() * 500) {
        cEditingIconCycle -= static_cast<float>(cOffsetModels.size()) * 500;
      }
    });
  }
  
  ModelCycler::ModelCycler(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            ModelCycler(project, basics, data) {
    unsigned int mIndex = 0;
    for (JSONObject mModelObject : object.getArray(JSON_MODELS)) {
      cDefModels.emplace_back(std::make_unique<Model>(data))->init(mModelObject, JSON_MODEL);
      cOffsetModels.emplace_back(std::make_unique<Offset>(*this, mIndex++));
    }
  }

  void ModelCycler::registerAssets(IAssetRegistry& assets) {
    for (unsigned int i = 0; i < cOffsetModels.size(); i++) {
      assets.add(cOffsetModels[i].get(), std::to_string(i), "Cycleable Models");
    }
    assets.add(&cLuaBinding, "", "Cycleable Models");
  }
  
  void ModelCycler::save(JSONObject object, IAssetIdentifier& identifier) const {
    JSONArray mModelArray = object.addArray(JSON_MODELS);
    for (const std::unique_ptr<Model>& mModel : cDefModels) {
      JSONObject mModelObject = mModelArray.addObject();
      mModel->save(mModelObject, JSON_MODEL);
    }
  }

  void ModelCycler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ModelCycler::renderIcon() {
    unsigned int mIndex = cEditingIconCycle / 500;
    return cDefModels[mIndex]->renderIcon();
  }

  std::vector<std::unique_ptr<IProperty>> ModelCycler::getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    unsigned int mModelCount = 1;
    for (const std::unique_ptr<Model>& mModel : cDefModels) {
      mProperties.emplace_back(std::make_unique<PropertyAsset<Model>>("Model " + Utils::toString(mModelCount), "TODO", *mModel.get()));
      mModelCount++;
    }
    return mProperties;
  }

  void ModelCycler::next() {
    if (++cRuntimeCycleIndex == cDefModels.size()) {
      cRuntimeCycleIndex = 0;
    }
  }

  void ModelCycler::previous() {
    if (cRuntimeCycleIndex-- == 0) {
      cRuntimeCycleIndex = static_cast<unsigned int>(cDefModels.size()) - 1;
    }
  }

  ModelCycler::Offset::Offset(ModelCycler& parent, unsigned int offset) :
            cParent(parent),
            cDefOffset(offset) {
  }

  unsigned int ModelCycler::Offset::getCycleIndex() const {
    return (cParent.cRuntimeCycleIndex + cDefOffset) % cParent.cDefModels.size();
  }

  IModelInstance* ModelCycler::Offset::createModel() {
    return cRuntimeInstances.emplace_back(std::make_unique<Instance>(*this)).get();
  }

  bool ModelCycler::Offset::renderPreview() const {
    return cParent.cDefModels[cDefOffset]->renderPreview();
  }

  bool ModelCycler::Offset::renderAssetIcon() const {
    return cParent.cDefModels[cDefOffset]->renderIcon();
  }

  void ModelCycler::Offset::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ModelCycler::Offset::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ModelCycler::Offset::isDefaultConfiguration() const {
    return true;
  }

  ModelCycler::Offset::Instance::Instance(Offset& parent) :
            cParent(parent) {
    for (std::unique_ptr<Model>& mModel : cParent.cParent.cDefModels) {
      cModels.emplace_back(mModel->createInstance());
    }
  }

  void ModelCycler::Offset::Instance::update(unsigned int milliseconds) {
    cModels[cParent.getCycleIndex()]->update(milliseconds);
  }

  void ModelCycler::Offset::Instance::render() const {
    cModels[cParent.getCycleIndex()]->render();
  }
}
