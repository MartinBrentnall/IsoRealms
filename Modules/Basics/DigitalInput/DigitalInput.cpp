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

namespace IsoRealms::Basics {
  const std::string DigitalInput::JSON_BUTTON_DOWN       = "ButtonDown";
  const std::string DigitalInput::JSON_HAT               = "Hat";
  const std::string DigitalInput::JSON_KEY_DOWN          = "KeyDown";
  const std::string DigitalInput::JSON_MAPPINGS          = "mappings";
  const std::string DigitalInput::JSON_MOUSE_BUTTON_DOWN = "MouseButtonDown";
  const std::string DigitalInput::JSON_TYPE              = "type";

  DigitalInput::DigitalInput(IProject& project, Basics& basics) :
             cProject(project),
             cRuntimeState(false),
             cLuaBinding(project, this),
             cStateNotifier(nullptr) {
  }

  DigitalInput::DigitalInput(IProject& project, Basics& basics, IResourceData& data) :
            DigitalInput(project, basics) {
  }
  
  DigitalInput::DigitalInput(IProject& project, Basics& basics, JSONObject object) :
            DigitalInput(project, basics) {
    IApplication& mApplication = project.getApplication();
    HatHandler& mHatHandler = mApplication.getHatHandler();
    for (JSONObject mMappingObject : object.getArray(JSON_MAPPINGS)) {
      std::string mType = mMappingObject.getString(JSON_TYPE);
      if      (mType == KeyMapping::TYPE_KEY_DOWN)                  {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<KeyMapping>(mMappingObject)));}
      else if (mType == ButtonMapping::TYPE_BUTTON_DOWN)            {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<ButtonMapping>(mMappingObject)));}
      else if (mType == HatMapping::TYPE_HAT)                       {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<HatMapping>(mHatHandler, mMappingObject)));}
      else if (mType == MouseButtonMapping::TYPE_MOUSE_BUTTON_DOWN) {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<MouseButtonMapping>(mMappingObject)));}
      else                                                          {throw ParseException("Unknown tag for Basics/DigitalInput: " + mType);}
    }
  }

  DigitalInput::DigitalInput(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            DigitalInput(project, basics, object) {
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

  void DigitalInput::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    for (std::unique_ptr<PhysicalInputMapping>& mInput : cDefMapping) {
      owner.createPropertyStruct(metadata.getPropertyData("Input"), mInput->getShortName(), [&mInput](PropertyMaker& owner) {
        return mInput->getProperties(owner);
      }, [this, &mInput]() {
        Utils::removeElementUnique(cDefMapping, mInput.get());
      });
    }
    
    owner.createPropertyAdd(metadata.getPropertyData("Input"), "Add...", [this, &owner, &metadata]() {
      cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<KeyMapping>(sf::Keyboard::Return)));
      std::unique_ptr<PhysicalInputMapping>& mInput = cDefMapping.back();
      return owner.createPropertyStruct(metadata.getPropertyData("Input"), mInput->getShortName(), [&mInput](PropertyMaker& owner) {
        return mInput->getProperties(owner);
      }, [this, &mInput]() {
        Utils::removeElementUnique(cDefMapping, mInput.get());
      });
    });
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
      cStateNotifier->stateChanged(this);
      return true;
    }
    return false;
  }

  void DigitalInput::resetInput() {
    if (cRuntimeState) {
      cRuntimeState = false;
      cStateNotifier->stateChanged(this);
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

  void DigitalInput::getAssetProperties(PropertyMaker& owner) {
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

  std::shared_ptr<IDigitalInputMapping> DigitalInput::getMapping(unsigned int index) const {
    const std::vector<std::unique_ptr<PhysicalInputMapping>>& mMapping = cRuntimeMapping.empty() ? cDefMapping : cRuntimeMapping;
    if (index < mMapping.size()) {
      return mMapping[index]->getInput();
    }
    throw ActionException("DigitalInput::getMapping(" + Utils::toString(index) + ") : Index out of range: 0..." + Utils::toString(static_cast<int>(mMapping.size()) - 1) + ")");
  }

  void DigitalInput::addCustomInput(std::shared_ptr<IDigitalInputMapping> input) {
    cRuntimeMapping.push_back(std::make_unique<PhysicalInputMapping>(input));
  }

  void DigitalInput::clearCustomInputs() {
    cRuntimeMapping.clear();
  }

  DigitalInput::PhysicalInputMapping::PhysicalInputMapping(std::shared_ptr<IDigitalInputMapping> physicalInput) :
            cPhysicalInput(physicalInput),
            cState(false) {
  }

  bool DigitalInput::PhysicalInputMapping::matches(sf::Event& event) const {
    return cPhysicalInput->matches(event);
  }

  bool DigitalInput::PhysicalInputMapping::input(sf::Event& event) {
    if (cPhysicalInput->matches(event)) {
      cState = cPhysicalInput->getState(event);
    }
    return cState;
  }

  void DigitalInput::PhysicalInputMapping::reset() {
    cState = false;
  }

  std::string DigitalInput::PhysicalInputMapping::getShortName() const {
    return cPhysicalInput->getShortName();
  }

  std::shared_ptr<IDigitalInputMapping> DigitalInput::PhysicalInputMapping::getInput() const {
    return cPhysicalInput;
  }

  void DigitalInput::PhysicalInputMapping::save(JSONObject object) const {
    cPhysicalInput->save(object);
  }

  void DigitalInput::PhysicalInputMapping::getProperties(PropertyMaker& owner) {
    cPhysicalInput->getProperties(owner);
  }

  void DigitalInput::loadCustomMapping(JSONObject object) {
    IApplication& mApplication = cProject.getApplication();
    HatHandler& mHatHandler = mApplication.getHatHandler();
    cRuntimeMapping.clear();
    for (JSONObject mMappingsObject : object.getArray(JSON_MAPPINGS)) {
      std::string mType = mMappingsObject.getString(JSON_TYPE);
      if      (mType == KeyMapping::TYPE_KEY_DOWN)                  {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<KeyMapping>(mMappingsObject)));}
      else if (mType == ButtonMapping::TYPE_BUTTON_DOWN)            {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<ButtonMapping>(mMappingsObject)));}
      else if (mType == HatMapping::TYPE_HAT)                       {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<HatMapping>(mHatHandler, mMappingsObject)));}
      else if (mType == MouseButtonMapping::TYPE_MOUSE_BUTTON_DOWN) {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<MouseButtonMapping>(mMappingsObject)));}
      else                                                          {throw ParseException("Unknown tag for Basics/DigitalInput: " + mType);}
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
