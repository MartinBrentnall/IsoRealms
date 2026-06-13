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
#include "AnalogueControl.h"

#include "Modules/Basics/Basics.h"

#include "IsoRealms/Project/Options.h"

namespace IsoRealms::Basics {
  AnalogueControl::AnalogueControl(Basics& basics, IComponentData& data) :
             cComponentData(data),
             cRuntimeState(false),
             cLuaBinding(basics.getProject().getLuaState(), this) {
  }

  AnalogueControl::AnalogueControl(Basics& basics, IComponentData& data, JSONObject object) :
            AnalogueControl(basics, data) {
    for (JSONValue mMappingValue : object.getArray(JSON_MAPPINGS)) {
      JSONObject mMappingObject = mMappingValue.getObject();
      std::shared_ptr<AnalogueInput> mInput = std::make_shared<AnalogueInput>(data);
      mInput->set(mMappingObject);
      cDefMapping.emplace_back(std::make_unique<InputMapping>(mInput));
    }
  }

  void AnalogueControl::registerAssets(ComponentAssetRegistry& assets) {
    cStateNotifier = assets.add<IFloat>(static_cast<IFloat*>(this), "", "Analogue Inputs");
    assets.add<IInputHandler>(static_cast<IInputHandler*>(this), "", "Analogue Inputs");
    assets.add<IBinding>(&cLuaBinding, "", "Analogue Inputs");
    for (std::unique_ptr<InputMapping>& mInput : cDefMapping) {
      mInput->registerAssets(assets);
    }
  }

  void AnalogueControl::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool AnalogueControl::renderIcon() const {
    return false;
  }

  void AnalogueControl::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray(JSON_MAPPINGS, cDefMapping, [](const std::unique_ptr<InputMapping>& mMapping)->InputMapping& {return *mMapping;}, [this, &owner](InputMapping& mapping) {
      mapping.getProperties(owner, [this, &mapping]() {
        Utils::removeElementUnique(cDefMapping, &mapping);
      });
    }, [this]()->InputMapping& {
      std::shared_ptr<AnalogueInput> mInput = std::make_shared<AnalogueInput>(cComponentData);
      mInput->setID("GameControllerAxis");
      return *cDefMapping.emplace_back(std::make_unique<InputMapping>(mInput));
    });
  }

  void AnalogueControl::removed() {
    // Nothing to do.
  }

  float AnalogueControl::getValue() const {
    return cRuntimeState;
  }

  bool AnalogueControl::input(sf::Event& event) {
    std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    float mValue = 0.0f;
    for (std::unique_ptr<InputMapping>& mInput : mMapping) {
      mValue += mInput->input(event);
    }
    mValue = std::clamp(mValue, -1.0f, 1.0f);

    if (mValue != cRuntimeState) {
      cRuntimeState = mValue;
      cStateNotifier->stateChanged();
      return true;
    }
    return false;
  }

  void AnalogueControl::resetInput() {
    if (cRuntimeState != 0.0f) {
      cRuntimeState = 0.0f;
      cStateNotifier->stateChanged();
    }

    std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    for (std::unique_ptr<InputMapping>& mInput : mMapping) {
      mInput->reset();
    }
  }

  bool AnalogueControl::renderAssetIcon() const {
    return renderIcon();
  }

  void AnalogueControl::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void AnalogueControl::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool AnalogueControl::isDefaultConfiguration() const {
    return true;
  }

  std::string AnalogueControl::getInputsString() const {
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

  unsigned int AnalogueControl::getMappingCount() const {
    return static_cast<unsigned int>(cRuntimeMapping.empty() ? cDefMapping.size() : cRuntimeMapping.size());
  }

  std::shared_ptr<AnalogueInput> AnalogueControl::getMapping(unsigned int index) const {
    const std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    if (index < mMapping.size()) {
      return mMapping[index]->getInput();
    }
    throw ActionException("AnalogueInput::getMapping(" + Utils::toString(index) + ") : Index out of range: 0..." + Utils::toString(static_cast<int>(mMapping.size()) - 1) + ")");
  }

  void AnalogueControl::addCustomInput(std::shared_ptr<AnalogueInput> input) {
    cRuntimeMapping.emplace_back(std::make_unique<InputMapping>(input));
  }

  void AnalogueControl::clearCustomInputs() {
    cRuntimeMapping.clear();
  }

  void AnalogueControl::loadCustomMapping(JSONObject object) {
    cRuntimeMapping.clear();
    for (JSONValue mMappingsValue : object.getArray(JSON_MAPPINGS)) {
      JSONObject mMappingsObject = mMappingsValue.getObject();
      std::string mMappingType = mMappingsObject.getString("type");
      if (mMappingType == "Input") {
        std::string mInputID = mMappingsObject.getString("id");
        for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
          if (mMapping->getName() == mInputID) {
            mMapping->loadCustomMapping(mMappingsObject);
            break;
          }
        }
      } else {
        throw ParseException("Unknown tag for Basics/AnalogueInput: " + mMappingType);
      }
    }
  }

  void AnalogueControl::saveCustomMapping(JSONObject object) const {
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
      JSONObject mMappingsObject = mMappingsArray.addObject();
      mMapping->save(mMappingsObject);
    }
    // TODO
//     for (const std::unique_ptr<InputMapping>& mMapping : cRuntimeMapping) {
//       mMapping->save(node);
//     }
  }

  AnalogueControl::InputMapping::InputMapping(std::shared_ptr<AnalogueInput> physicalInput) :
            cPhysicalInput(physicalInput),
            cState(0.0f) {
  }

  bool AnalogueControl::InputMapping::matches(sf::Event& event) const {
    return (*cPhysicalInput)->matches(event);
  }

  float AnalogueControl::InputMapping::input(sf::Event& event) {
    if ((*cPhysicalInput)->matches(event)) {
      cState = (*cPhysicalInput)->getState(event);
    }
    return cState;
  }

  void AnalogueControl::InputMapping::save(JSONObject object) const {
    cPhysicalInput->save(object);
  }

  void AnalogueControl::InputMapping::loadCustomMapping(JSONObject object) {
    (*cPhysicalInput)->loadCustomMapping(object);
  }

  void AnalogueControl::InputMapping::registerAssets(ComponentAssetRegistry& assets) {
    (*cPhysicalInput)->registerAssets(assets);
  }
  
  std::string AnalogueControl::InputMapping::getShortName() const {
    return (*cPhysicalInput)->getShortName();
  }

  std::shared_ptr<AnalogueInput> AnalogueControl::InputMapping::getInput() const {
    return cPhysicalInput;
  }

  std::string AnalogueControl::InputMapping::getName() {
    return (*cPhysicalInput)->getName();
  }

  void AnalogueControl::InputMapping::getProperties(IPropertyMaker& owner, std::function<void()> removeFunction) {
    Options mHint;
    mHint.addOption(Options::PROPERTY_INLINE, "true");
    mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
    owner.createPropertyTreeSelector("DefaultMapping", *cPhysicalInput, mHint, removeFunction);
  }

  void AnalogueControl::InputMapping::reset() {
    cState = 0.0f;
  }
}

