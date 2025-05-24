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
#include "AnalogueInput.h"

namespace IsoRealms::Basics {
  const std::string AnalogueInput::JSON_ID       = "id";
  const std::string AnalogueInput::JSON_INPUT    = "Input";
  const std::string AnalogueInput::JSON_MAPPINGS = "mappings";
  const std::string AnalogueInput::JSON_NAME     = "name";
  const std::string AnalogueInput::JSON_TYPE     = "type";

  AnalogueInput::AnalogueInput(IProject& project, Basics& basics, IResourceData& data) :
             cRuntimeState(false),
             cLuaBinding(project, this) {
  }

  AnalogueInput::AnalogueInput(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            AnalogueInput(project, basics, data) {
    for (JSONObject mMappingObject : object.getArray(JSON_MAPPINGS)) {
      std::string mType = mMappingObject.getString(JSON_TYPE);
      if      (mType == AxisMapping::TYPE_AXIS)                             {cDefMapping.emplace_back(std::make_unique<InputMapping>(std::make_shared<AxisMapping>(mMappingObject), "TODO: Remove This"));}
      else if (mType == DigitalToAnalogueMapping::TYPE_DIGITAL_TO_ANALOGUE) {cDefMapping.emplace_back(std::make_unique<InputMapping>(std::make_shared<DigitalToAnalogueMapping>(project, basics, mMappingObject), mMappingObject.getString(JSON_NAME)));}
      else                                                                  {throw ParseException("Unknown tag for Basics/AnalogueInput: " + mType);}
    }
  }

  void AnalogueInput::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(static_cast<IFloat*>(this), "", "Analogue Inputs");
    assets.add(static_cast<IInputHandler*>(this), "", "Analogue Inputs");
    assets.add(&cLuaBinding, "", "Analogue Inputs");
    for (std::unique_ptr<InputMapping>& mInput : cDefMapping) {
      mInput->registerAssets(assets);
    }
  }

  void AnalogueInput::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(static_cast<IFloat*>(this),        relinquish);
    assets.remove(static_cast<IInputHandler*>(this), relinquish);
    assets.remove(&cLuaBinding,                      relinquish);
    for (std::unique_ptr<InputMapping>& mInput : cDefMapping) {
      mInput->unregisterAssets(assets, releaser, relinquish);
    }
  }

  void AnalogueInput::save(JSONObject object, IAssetIdentifier& identifier) const {
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
      JSONObject mMappingObject = mMappingsArray.addObject();
      mMapping->save(mMappingObject);
    }
  }

  void AnalogueInput::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool AnalogueInput::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> AnalogueInput::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  float AnalogueInput::getValue() const {
    return cRuntimeState;
  }

  bool AnalogueInput::input(sf::Event& event) {
    std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    float mValue = 0.0f;
    for (std::unique_ptr<InputMapping>& mInput : mMapping) {
      mValue += mInput->input(event);
    }
    mValue = std::clamp(mValue, -1.0f, 1.0f);

    if (mValue != cRuntimeState) {
      cRuntimeState = mValue;
      cStateNotifier->stateChanged(this);
      return true;
    }
    return false;
  }

  void AnalogueInput::resetInput() {
    if (cRuntimeState != 0.0f) {
      cRuntimeState = 0.0f;
      cStateNotifier->stateChanged(this);
    }

    std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    for (std::unique_ptr<InputMapping>& mInput : mMapping) {
      mInput->reset();
    }
  }

  bool AnalogueInput::renderAssetIcon() const {
    return renderIcon();
  }

  void AnalogueInput::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> AnalogueInput::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool AnalogueInput::isDefaultConfiguration() const {
    return true;
  }

  std::string AnalogueInput::getInputsString() const {
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

  unsigned int AnalogueInput::getMappingCount() const {
    return static_cast<unsigned int>(cRuntimeMapping.empty() ? cDefMapping.size() : cRuntimeMapping.size());
  }

  std::shared_ptr<IAnalogueInputMapping> AnalogueInput::getMapping(unsigned int index) const {
    const std::vector<std::unique_ptr<InputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    if (index < mMapping.size()) {
      return mMapping[index]->getInput();
    }
    throw ActionException("AnalogueInput::getMapping(" + Utils::toString(index) + ") : Index out of range: 0..." + Utils::toString(static_cast<int>(mMapping.size()) - 1) + ")");
  }

  void AnalogueInput::addCustomInput(std::shared_ptr<IAnalogueInputMapping> input) {
    cRuntimeMapping.emplace_back(std::make_unique<InputMapping>(input, "TODO: Custom Analogue Mapping"));
  }

  void AnalogueInput::clearCustomInputs() {
    cRuntimeMapping.clear();
  }

  void AnalogueInput::loadCustomMapping(JSONObject object) {
    cRuntimeMapping.clear();
    for (JSONObject mMappingsObject : object.getArray(JSON_MAPPINGS)) {
      std::string mMappingType = mMappingsObject.getString(JSON_TYPE);
      if (mMappingType == JSON_INPUT) {
        std::string mInputID = mMappingsObject.getString(JSON_ID);
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

  void AnalogueInput::saveCustomMapping(JSONObject object) const {
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

  AnalogueInput::InputMapping::InputMapping(std::shared_ptr<IAnalogueInputMapping> physicalInput, const std::string& name) :
            cName(name),
            cPhysicalInput(physicalInput),
            cState(0.0f) {
  }

  bool AnalogueInput::InputMapping::matches(sf::Event& event) const {
    return cPhysicalInput->matches(event);
  }

  float AnalogueInput::InputMapping::input(sf::Event& event) {
    if (cPhysicalInput->matches(event)) {
      cState = cPhysicalInput->getState(event);
    }
    return cState;
  }

  void AnalogueInput::InputMapping::save(JSONObject object) const {
    cPhysicalInput->save(object, cName);
  }

  void AnalogueInput::InputMapping::loadCustomMapping(JSONObject object) {
    cPhysicalInput->loadCustomMapping(object);
  }

  void AnalogueInput::InputMapping::registerAssets(IAssetRegistry& assets) {
    LocalAssetRegistry mLocalRegistry(assets, cName);
    cPhysicalInput->registerAssets(mLocalRegistry);
  }
  
  void AnalogueInput::InputMapping::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    cPhysicalInput->unregisterAssets(assets, releaser, relinquish);
  }

  std::string AnalogueInput::InputMapping::getShortName() const {
    return cPhysicalInput->getShortName();
  }

  std::shared_ptr<IAnalogueInputMapping> AnalogueInput::InputMapping::getInput() const {
    return cPhysicalInput;
  }

  std::string AnalogueInput::InputMapping::getName() {
    return cName;
  }

  void AnalogueInput::InputMapping::reset() {
    cState = 0.0f;
  }
}

