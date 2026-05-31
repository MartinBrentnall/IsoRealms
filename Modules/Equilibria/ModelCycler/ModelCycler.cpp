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
#include "ModelCycler.h"

namespace IsoRealms::Equilibria {
  ModelCycler::ModelCycler(Equilibria& equilibria, IResourceData& data) :
            cRuntimeCycleIndex(0),
            cLuaBinding(data.getProject().getLuaState(), this),
            cEditingIconCycle(0) {
  }
  
  ModelCycler::ModelCycler(Equilibria& equilibria, IResourceData& data, JSONObject object) :
            ModelCycler(equilibria, data) {
    unsigned int mIndex = 0;
    for (JSONValue mModelValue : object.getArray(JSON_MODELS)) {
      cDefModels.emplace_back(std::make_unique<Model>(data))->init(mModelValue.getObject(), JSON_MODEL);
      cOffsetModels.emplace_back(std::make_unique<Offset>(*this, mIndex++));
    }
  }

  void ModelCycler::registerAssets(ResourceAssetRegistry& assets) {
    for (unsigned int i = 0; i < cOffsetModels.size(); i++) {
      assets.add<IModel>(cOffsetModels[i].get(), std::to_string(i), "Cycleable Models");
    }
    assets.add<IBinding>(&cLuaBinding, "", "Cycleable Models");
  }
  
  void ModelCycler::save(JSONObject object) const {
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

  void ModelCycler::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    for (const std::unique_ptr<Model>& mModel : cDefModels) {
      owner.createPropertyTreeSelector(metadata.getPropertyData("Model"), *mModel.get());
    }
  }

  void ModelCycler::removed() {
    // Nothing to do.
  }
  
  void ModelCycler::updateEditing(unsigned int milliseconds) {
    cEditingIconCycle += milliseconds;
    if (cEditingIconCycle >= cOffsetModels.size() * 500) {
      cEditingIconCycle -= static_cast<float>(cOffsetModels.size()) * 500;
    }
  }
    
  void ModelCycler::reset() {
    cRuntimeCycleIndex = 0; 
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

  void ModelCycler::Offset::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
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
