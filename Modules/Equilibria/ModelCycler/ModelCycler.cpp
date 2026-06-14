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

#include "Modules/Equilibria/Equilibria.h"

#include "IsoRealms/ComponentLoader.h"

namespace IsoRealms::Equilibria {
  ModelCycler::ModelCycler(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cComponentData(data),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void ModelCycler::define(IComponentDefiner& definer) {
    definer.array("models", cDefModels, [](const std::unique_ptr<Model>& mModel) -> Model& {return *mModel;}, [this, &definer](Model& model) {
      definer.propertyResource("model", model, Options::EMPTY, [this, &model]() {
        Utils::removeElementUnique(cDefModels, &model);
        rebuildOffsetModels();
        refreshResourceRegistration();
      });
    }, [this]() -> Model& {
      Model& mModel = *cDefModels.emplace_back(std::make_unique<Model>(cComponentData));
      rebuildOffsetModels();
      return mModel;
    });
  }

  void ModelCycler::publish(ResourcePublisher& publisher) {
    for (unsigned int i = 0; i < cOffsetModels.size(); i++) {
      publisher.publish<IModel>(cOffsetModels[i].get(), std::to_string(i), "Cycleable Models");
    }
    publisher.publish<IBinding>(&cLuaBinding, "", "Cycleable Models");
  }

  bool ModelCycler::renderIcon() {
    unsigned int mIndex = cEditingIconCycle / 500;
    return cDefModels[mIndex]->renderIcon();
  }

  void ModelCycler::rebuildOffsetModels() {
    cOffsetModels.clear();
    for (unsigned int i = 0; i < cDefModels.size(); i++) {
      cOffsetModels.emplace_back(std::make_unique<Offset>(*this, i));
    }
  }

  void ModelCycler::refreshResourceRegistration() {
    ComponentContainerTraits<ModelCycler>::get(cEquilibria).refreshResourceRegistration(*this);
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

  bool ModelCycler::Offset::renderResourceIcon() const {
    return cParent.cDefModels[cDefOffset]->renderIcon();
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
