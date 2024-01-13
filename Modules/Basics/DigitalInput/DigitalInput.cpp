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
#include "DigitalInput.h"

namespace IsoRealms::Basics {
  const std::string DigitalInput::TAG_BUTTON_DOWN   = "ButtonDown";
  const std::string DigitalInput::TAG_HAT           = "Hat";
  const std::string DigitalInput::TAG_KEY_DOWN      = "KeyDown";

  DigitalInput::DigitalInput(IProject* project, Basics* basics) :
             cProject(project),
             cRuntimeState(false),
             cLuaBinding(project, this),
             cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeState = false;
      for (std::unique_ptr<PhysicalInputMapping>& mMapping : cDefMapping) {
        mMapping->reset();
      }
      for (std::unique_ptr<PhysicalInputMapping>& mMapping : cRuntimeMapping) {
        mMapping->reset();
      }
    });
  }
  
  DigitalInput::DigitalInput(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            DigitalInput(project, basics) {
    IApplication* mApplication = project->getApplication();
    HatHandler& mHatHandler = mApplication->getHatHandler();
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if      (mChildName == TAG_KEY_DOWN)    {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<KeyMapping>(mNode)));}
      else if (mChildName == TAG_BUTTON_DOWN) {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<ButtonMapping>(mNode)));}
      else if (mChildName == TAG_HAT)         {cDefMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<HatMapping>(mHatHandler, mNode)));}
      else                                    {throw ParseException("Unknown tag for Basics/DigitalInput: " + mChildName);}
    }
  }
  
  void DigitalInput::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(static_cast<IBoolean*>(this), "", "Digital Inputs");
    assets->add(static_cast<IInputHandler*>(this), "", "Digital Inputs");
    assets->add(&cLuaBinding, "", "Digital Inputs");
  }
  
  void DigitalInput::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IBoolean*>(this));
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }
  
  void DigitalInput::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::unique_ptr<PhysicalInputMapping>& mMapping : cDefMapping) {
      mMapping->save(node);
    }
  }

  void DigitalInput::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool DigitalInput::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> DigitalInput::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  bool DigitalInput::renderAssetIcon() const {
    return renderIcon();
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

  void DigitalInput::PhysicalInputMapping::save(DOMNodeWriter* node) const {
    cPhysicalInput->save(node);
  }

  void DigitalInput::loadCustomMapping(DOMNode& node) {
    IApplication* mApplication = cProject->getApplication();
    HatHandler& mHatHandler = mApplication->getHatHandler();
    cRuntimeMapping.clear();
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if      (mChildName == TAG_KEY_DOWN)    {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<KeyMapping>(mNode)));}
      else if (mChildName == TAG_BUTTON_DOWN) {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<ButtonMapping>(mNode)));}
      else if (mChildName == TAG_HAT)         {cRuntimeMapping.emplace_back(std::make_unique<PhysicalInputMapping>(std::make_shared<HatMapping>(mHatHandler, mNode)));}
      else                                    {throw ParseException("Unknown tag for Basics/DigitalInput: " + mChildName);}
    }
  }

  void DigitalInput::saveCustomMapping(DOMNodeWriter* node) const {
    for (const std::unique_ptr<PhysicalInputMapping>& mMapping : cRuntimeMapping) {
      mMapping->save(node);
    }
  }
}
