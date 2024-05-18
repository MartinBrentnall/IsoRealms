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

  ModelCycler::ModelCycler(IProject* project, Basics* basics) :
            cRuntimeCycleIndex(0),
            cLuaBinding(project, this),
            cEditingIconCycle(0) {
    project->reset([this]() {
      cRuntimeCycleIndex = 0; 
    });
    
    project->updateEditing([this](unsigned int milliseconds) {
      cEditingIconCycle += milliseconds;
      if (cEditingIconCycle >= cOffsetModels.size() * 500) {
        cEditingIconCycle -= static_cast<float>(cOffsetModels.size()) * 500;
      }
    });
  }
  
  ModelCycler::ModelCycler(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            ModelCycler(project, basics) {
    unsigned int mIndex = 0;
    for (JSONObject mModelObject : object.getArray(JSON_MODELS)) {
      cDefModelTypes.emplace_back(std::make_unique<Model>(project))->init(mModelObject, JSON_MODEL);
      cOffsetModels.emplace_back(std::make_unique<Offset>(this, mIndex++));
    }
  }

  void ModelCycler::registerAssets(IAssetRegistry* assets) {
    for (unsigned int i = 0; i < cOffsetModels.size(); i++) {
      assets->add(cOffsetModels[i].get(), std::to_string(i), "Cycleable Models");
    }
    assets->add(&cLuaBinding, "", "Cycleable Models");
  }
  
  void ModelCycler::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    for (std::unique_ptr<Offset>& mOffsetModel : cOffsetModels) {
      assets->remove(mOffsetModel.get());
    }
    assets->remove(&cLuaBinding);
  }

  void ModelCycler::save(JSONObject object, IAssetIdentifier* identifier) const {
    JSONArray mModelArray = object.addArray(JSON_MODELS);
    for (const std::unique_ptr<Model>& mModelType : cDefModelTypes) {
      JSONObject mModelObject = mModelArray.addObject();
      mModelType->save(mModelObject, JSON_MODEL);
    }
  }

  void ModelCycler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ModelCycler::renderIcon() {
    unsigned int mIndex = cEditingIconCycle / 500;
    return cDefModelTypes[mIndex]->renderIcon();
  }

  std::vector<IProperty*> ModelCycler::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void ModelCycler::next() {
    if (++cRuntimeCycleIndex == cDefModelTypes.size()) {
      cRuntimeCycleIndex = 0;
    }
  }

  void ModelCycler::previous() {
    if (cRuntimeCycleIndex-- == 0) {
      cRuntimeCycleIndex = static_cast<unsigned int>(cDefModelTypes.size()) - 1;
    }
  }

  ModelCycler::Offset::Offset(ModelCycler* parent, unsigned int offset) :
            cParent(parent),
            cDefOffset(offset) {
  }

  unsigned int ModelCycler::Offset::getCycleIndex() const {
    return (cParent->cRuntimeCycleIndex + cDefOffset) % cParent->cDefModelTypes.size();
  }

  I3DModel* ModelCycler::Offset::createModel() {
    return cRuntimeInstances.emplace_back(std::make_unique<Instance>(this)).get();
  }

  bool ModelCycler::Offset::renderPreview() const {
    return cParent->cDefModelTypes[cDefOffset]->renderPreview();
  }

  bool ModelCycler::Offset::renderAssetIcon() const {
    return cParent->cDefModelTypes[cDefOffset]->renderIcon();
  }

  void ModelCycler::Offset::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  ModelCycler::Offset::Instance::Instance(Offset* parent) :
            cParent(parent) {
    for (std::unique_ptr<Model>& mModel : cParent->cParent->cDefModelTypes) {
      cModels.emplace_back(mModel->createInstance());
    }
  }

  void ModelCycler::Offset::Instance::update(unsigned int milliseconds) {
    cModels[cParent->getCycleIndex()]->update(milliseconds);
  }

  void ModelCycler::Offset::Instance::render() const {
    cModels[cParent->getCycleIndex()]->render();
  }
}
