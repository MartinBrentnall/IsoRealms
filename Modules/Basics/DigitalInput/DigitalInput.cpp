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
#include "DigitalInput.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  DigitalInput::DigitalInput(IResourceData& owner) :
            cProject(owner.getProject()),
            cResourceData(owner),
            cRuntimeState(false),
            cLuaBinding(owner.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }

  DigitalInput::DigitalInput(IResourceData& owner, JSONObject object) :
            DigitalInput(owner) {
    for (JSONValue mMappingValue : object.getArray(JSON_MAPPINGS)) {
      JSONObject mMappingObject = mMappingValue.getObject();
      std::shared_ptr<DigitalInputMapping> mInput = std::make_shared<DigitalInputMapping>(owner);
      mInput->set(mMappingObject);
      cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(mInput));
    }
  }

  DigitalInput::DigitalInput(Basics& basics, IResourceData& data) :
            DigitalInput(data) {
  }
  
  DigitalInput::DigitalInput(Basics& basics, IResourceData& data, JSONObject object) :
            DigitalInput(data, object) {
  }

  void DigitalInput::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IBoolean>(this,         "", "Digital Inputs");
    assets.add<IInputHandler>(            this,         "", "Digital Inputs");
    assets.add<IBinding>(                 &cLuaBinding, "", "Digital Inputs");
  }

  void DigitalInput::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    cStateNotifier = assets.add<IBoolean>(this,         parentID, "Digital Inputs");
    assets.add<IInputHandler>(            this,         parentID, "Digital Inputs");
    assets.add<IBinding>(                 &cLuaBinding, parentID, "Digital Inputs");
  }
  
  void DigitalInput::save(JSONObject object) const {
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::unique_ptr<PhysicalInputMapping>& mMapping : cDefMapping) {
      JSONObject mMappingObject = mMappingsArray.addObject();
      mMapping->save(mMappingObject);
    }
  }

  void DigitalInput::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool DigitalInput::renderIcon() const {
    return false;
  }

  void DigitalInput::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    // for (std::unique_ptr<PhysicalInputMapping>& mInput : cDefMapping) {
    //   owner.createPropertyStruct(metadata.getPropertyData("Input"), mInput->getShortName(), [&mInput](IPropertyMaker& owner) {
    //     return mInput->getProperties(owner);
    //   }, [this, &mInput]() {
    //     Utils::removeElementUnique(cDefMapping, mInput.get());
    //   });
    // }
    
    // owner.createPropertyAdd(metadata.getPropertyData("Input"), "Add...", [this, &owner, &metadata]() {
    //   cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<KeyMapping>(sf::Keyboard::Return)));
    //   std::unique_ptr<PhysicalInputMapping>& mInput = cDefMapping.back();
    //   return owner.createPropertyStruct(metadata.getPropertyData("Input"), mInput->getShortName(), [&mInput](IPropertyMaker& owner) {
    //     return mInput->getProperties(owner);
    //   }, [this, &mInput]() {
    //     Utils::removeElementUnique(cDefMapping, mInput.get());
    //   });
    // });
  }

  void DigitalInput::removed() {
    // Nothing to do.
  }

  void DigitalInput::reset() {
    cRuntimeState = false;
    for (std::unique_ptr<PhysicalInputMapping>& mMapping : cDefMapping) {
      mMapping->reset();
    }
    for (std::unique_ptr<PhysicalInputMapping>& mMapping : cRuntimeMapping) {
      mMapping->reset();
    }
  }
  
  bool DigitalInput::getValue() const {
    return cRuntimeState;
  }
  
  bool DigitalInput::input(sf::Event& event) {
    std::vector<std::unique_ptr<PhysicalInputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    bool mActive = false;
    for (std::unique_ptr<PhysicalInputMapping>& mInput : mMapping) {
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

  void DigitalInput::resetInput() {
    if (cRuntimeState) {
      cRuntimeState = false;
      cStateNotifier->stateChanged();
    }

    std::vector<std::unique_ptr<PhysicalInputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    for (std::unique_ptr<PhysicalInputMapping>& mInput : mMapping) {
      mInput->reset();
    }
  }

  bool DigitalInput::renderAssetIcon() const {
    return renderIcon();
  }

  void DigitalInput::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DigitalInput::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool DigitalInput::isDefaultConfiguration() const {
    return true;
  }

  std::string DigitalInput::getInputsString() const {
    const std::vector<std::unique_ptr<PhysicalInputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    std::string mInputsString;
    for (const std::unique_ptr<PhysicalInputMapping>& mInput : mMapping) {
      if (!mInputsString.empty()) {
        mInputsString += ", ";
      }
      mInputsString += mInput->getShortName();
    }
    return mInputsString;
  }

  unsigned int DigitalInput::getMappingCount() const {
    return static_cast<unsigned int>(cRuntimeMapping.empty() ? cDefMapping.size() : cRuntimeMapping.size());
  }

  std::shared_ptr<DigitalInputMapping> DigitalInput::getMapping(unsigned int index) const {
    const std::vector<std::unique_ptr<PhysicalInputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    if (index < mMapping.size()) {
      return mMapping[index]->getInput();
    }
    throw ActionException("DigitalInput::getMapping(" + Utils::toString(index) + ") : Index out of range: 0..." + Utils::toString(static_cast<int>(mMapping.size()) - 1) + ")");
  }

  void DigitalInput::addCustomInput(std::shared_ptr<DigitalInputMapping> input) {
    cRuntimeMapping.push_back(std::make_unique<PhysicalInputMapping>(input));
  }

  void DigitalInput::clearCustomInputs() {
    cRuntimeMapping.clear();
  }

  DigitalInput::PhysicalInputMapping::PhysicalInputMapping(std::shared_ptr<DigitalInputMapping> physicalInput) :
            cPhysicalInput(physicalInput),
            cState(false) {
  }

  bool DigitalInput::PhysicalInputMapping::matches(sf::Event& event) const {
    return (*cPhysicalInput)->matches(event);
  }

  bool DigitalInput::PhysicalInputMapping::input(sf::Event& event) {
    if ((*cPhysicalInput)->matches(event)) {
      cState = (*cPhysicalInput)->getState(event);
    }
    return cState;
  }

  void DigitalInput::PhysicalInputMapping::reset() {
    cState = false;
  }

  std::string DigitalInput::PhysicalInputMapping::getShortName() const {
    return (*cPhysicalInput)->getShortName();
  }

  std::shared_ptr<DigitalInputMapping> DigitalInput::PhysicalInputMapping::getInput() const {
    return cPhysicalInput;
  }

  void DigitalInput::PhysicalInputMapping::save(JSONObject object) const {
    cPhysicalInput->save(object);
  }

  void DigitalInput::PhysicalInputMapping::getProperties(IPropertyMaker& owner) {
    // cPhysicalInput->getProperties(owner); TODO: Implement this.
  }

  void DigitalInput::loadCustomMapping(JSONObject object) {
    cRuntimeMapping.clear();
    for (JSONValue mMappingsValue : object.getArray(JSON_MAPPINGS)) {
      JSONObject mMappingObject = mMappingsValue.getObject();
      std::shared_ptr<DigitalInputMapping> mInput = std::make_shared<DigitalInputMapping>(cResourceData);
      mInput->set(mMappingObject);
      cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(mInput));
    }
  }

  void DigitalInput::saveCustomMapping(JSONObject object) const {
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::unique_ptr<PhysicalInputMapping>& mMapping : cRuntimeMapping) {
      JSONObject mMappingsObject = mMappingsArray.addObject();
      mMapping->save(mMappingsObject);
    }
  }
}
