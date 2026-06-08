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
#include "DigitalControl.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  DigitalControl::DigitalControl(IResourceData& owner) :
            cProject(owner.getProject()),
            cResourceData(owner),
            cRuntimeState(false),
            cLuaBinding(owner.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }

  DigitalControl::DigitalControl(IResourceData& owner, JSONObject object) :
            DigitalControl(owner) {
    for (JSONValue mMappingValue : object.getArray(JSON_MAPPINGS)) {
      JSONObject mMappingObject = mMappingValue.getObject();
      std::shared_ptr<DigitalInput> mInput = std::make_shared<DigitalInput>(owner);
      mInput->set(mMappingObject);
      cDefMapping.emplace_back(std::make_unique<InputMapping>(mInput));
    }
  }

  DigitalControl::DigitalControl(Basics& basics, IResourceData& data) :
            DigitalControl(data) {
  }
  
  DigitalControl::DigitalControl(Basics& basics, IResourceData& data, JSONObject object) :
            DigitalControl(data, object) {
  }

  void DigitalControl::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IBoolean>(this,         "", "Digital Inputs");
    assets.add<IInputHandler>(            this,         "", "Digital Inputs");
    assets.add<IBinding>(                 &cLuaBinding, "", "Digital Inputs");
  }

  void DigitalControl::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    cStateNotifier = assets.add<IBoolean>(this,         parentID, "Digital Inputs");
    assets.add<IInputHandler>(            this,         parentID, "Digital Inputs");
    assets.add<IBinding>(                 &cLuaBinding, parentID, "Digital Inputs");
  }
  
  void DigitalControl::save(JSONObject object) const {
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
      JSONObject mMappingObject = mMappingsArray.addObject();
      mMapping->save(mMappingObject);
    }
  }

  void DigitalControl::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool DigitalControl::renderIcon() const {
    return false;
  }

  void DigitalControl::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray("DefaultMappingAdd", cDefMapping, [](const std::unique_ptr<InputMapping>& mMapping)->InputMapping& {return *mMapping;}, [this, &owner](InputMapping& mapping) {
      mapping.getProperties(owner, [this, &mapping]() {
        Utils::removeElementUnique(cDefMapping, &mapping);
      });
    }, [this]()->InputMapping& {
      std::shared_ptr<DigitalInput> mInput = std::make_shared<DigitalInput>(cResourceData);
      mInput->setID("KeyboardKey");
      return *cDefMapping.emplace_back(std::make_unique<InputMapping>(mInput));
    });
  }

  void DigitalControl::removed() {
    // Nothing to do.
  }

  void DigitalControl::reset() {
    cRuntimeState = false;
    for (std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
      mMapping->reset();
    }
    for (std::unique_ptr<InputMapping>& mMapping : cRuntimeMapping) {
      mMapping->reset();
    }
  }
  
  bool DigitalControl::getValue() const {
    return cRuntimeState;
  }
  
  bool DigitalControl::input(sf::Event& event) {
    std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    bool mActive = false;
    for (std::unique_ptr<InputMapping>& mInput : mMapping) {
      if (mInput->input(event)) {
        mActive = true;
      }
    }

    if (mActive != cRuntimeState) {
      cRuntimeState = mActive;
      cStateNotifier->stateChanged();
      return true;
    }
    return false;
  }

  void DigitalControl::resetInput() {
    if (cRuntimeState) {
      cRuntimeState = false;
      cStateNotifier->stateChanged();
    }

    std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    for (std::unique_ptr<InputMapping>& mInput : mMapping) {
      mInput->reset();
    }
  }

  bool DigitalControl::renderAssetIcon() const {
    return renderIcon();
  }

  void DigitalControl::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DigitalControl::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool DigitalControl::isDefaultConfiguration() const {
    return true;
  }

  std::string DigitalControl::getInputsString() const {
    const std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    std::string mInputsString;
    for (const std::unique_ptr<InputMapping>& mInput : mMapping) {
      if (!mInputsString.empty()) {
        mInputsString += ", ";
      }
      mInputsString += mInput->getShortName();
    }
    return mInputsString;
  }

  unsigned int DigitalControl::getMappingCount() const {
    return static_cast<unsigned int>(cRuntimeMapping.empty() ? cDefMapping.size() : cRuntimeMapping.size());
  }

  std::shared_ptr<DigitalInput> DigitalControl::getMapping(unsigned int index) const {
    const std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    if (index < mMapping.size()) {
      return mMapping[index]->getInput();
    }
    throw ActionException("DigitalInput::getMapping(" + Utils::toString(index) + ") : Index out of range: 0..." + Utils::toString(static_cast<int>(mMapping.size()) - 1) + ")");
  }

  void DigitalControl::addCustomInput(std::shared_ptr<DigitalInput> input) {
    cRuntimeMapping.push_back(std::make_unique<InputMapping>(input));
  }

  void DigitalControl::clearCustomInputs() {
    cRuntimeMapping.clear();
  }

  DigitalControl::InputMapping::InputMapping(std::shared_ptr<DigitalInput> input) :
            cInput(input),
            cState(false) {
  }

  bool DigitalControl::InputMapping::matches(sf::Event& event) const {
    return (*cInput)->matches(event);
  }

  bool DigitalControl::InputMapping::input(sf::Event& event) {
    if ((*cInput)->matches(event)) {
      cState = (*cInput)->getState(event);
    }
    return cState;
  }

  void DigitalControl::InputMapping::reset() {
    cState = false;
  }

  std::string DigitalControl::InputMapping::getShortName() const {
    return (*cInput)->getShortName();
  }

  std::shared_ptr<DigitalInput> DigitalControl::InputMapping::getInput() const {
    return cInput;
  }

  void DigitalControl::InputMapping::save(JSONObject object) const {
    cInput->save(object);
  }

  void DigitalControl::InputMapping::getProperties(IPropertyMaker& owner, std::function<void()> removeFunction) {
    owner.createPropertyTreeSelector("DefaultMapping", *cInput, Options::EMPTY, removeFunction);
  }

  void DigitalControl::loadCustomMapping(JSONObject object) {
    cRuntimeMapping.clear();
    for (JSONValue mMappingsValue : object.getArray(JSON_MAPPINGS)) {
      JSONObject mMappingObject = mMappingsValue.getObject();
      std::shared_ptr<DigitalInput> mInput = std::make_shared<DigitalInput>(cResourceData);
      mInput->set(mMappingObject);
      cRuntimeMapping.emplace_back(std::make_unique<InputMapping>(mInput));
    }
  }

  void DigitalControl::saveCustomMapping(JSONObject object) const {
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::unique_ptr<InputMapping>& mMapping : cRuntimeMapping) {
      JSONObject mMappingsObject = mMappingsArray.addObject();
      mMapping->save(mMappingsObject);
    }
  }
}
