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
  const std::string AnalogueInput::TAG_AXIS                = "Axis";
  const std::string AnalogueInput::TAG_DIGITAL_TO_ANALOGUE = "DigitalToAnalogue";
  const std::string AnalogueInput::TAG_INPUT               = "Input";

  const std::string AnalogueInput::ATTRIBUTE_ID   = "id";
  const std::string AnalogueInput::ATTRIBUTE_NAME = "name";

  AnalogueInput::AnalogueInput(IProject* project, Basics* basics) :
             cRuntimeState(false),
             cLuaBinding(project, this) {
  }

  AnalogueInput::AnalogueInput(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            AnalogueInput(project, basics) {
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if      (mChildName == TAG_AXIS)                {cDefMapping.emplace_back(std::make_unique<InputMapping>(std::make_shared<AxisMapping>(mNode), mNode.getAttribute((ATTRIBUTE_NAME))));}
      else if (mChildName == TAG_DIGITAL_TO_ANALOGUE) {cDefMapping.emplace_back(std::make_unique<InputMapping>(std::make_shared<DigitalToAnalogueMapping>(project, mNode), mNode.getAttribute(ATTRIBUTE_NAME)));}
      else                                            {throw ParseException("Unknown tag for Basics/AnalogueInput: " + mChildName);}
    }
  }

  void AnalogueInput::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(static_cast<IFloat*>(this), "", "Analogue Inputs");
    assets->add(static_cast<IInputHandler*>(this), "", "Analogue Inputs");
    assets->add(&cLuaBinding, "", "Analogue Inputs");
    for (std::unique_ptr<InputMapping>& mInput : cDefMapping) {
      mInput->registerAssets(assets);
    }
  }

  void AnalogueInput::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IFloat*>(this));
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(&cLuaBinding);
    for (std::unique_ptr<InputMapping>& mInput : cDefMapping) {
      mInput->unregisterAssets(assets, releaser);
    }
  }

  void AnalogueInput::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
      mMapping->save(node);
    }
  }

  void AnalogueInput::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool AnalogueInput::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> AnalogueInput::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  bool AnalogueInput::renderAssetIcon() const {
    return renderIcon();
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

  void AnalogueInput::loadCustomMapping(DOMNode& node) {
    cRuntimeMapping.clear();
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == TAG_INPUT) {
        std::string mInputID = mNode.getAttribute(ATTRIBUTE_ID);
        for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
          if (mMapping->getName() == mInputID) {
            mMapping->loadCustomMapping(mNode);
            break;
          }
        }
      } else {
        throw ParseException("Unknown tag for Basics/AnalogueInput: " + mChildName);
      }
    }
  }

  void AnalogueInput::saveCustomMapping(DOMNodeWriter* node) const {
    for (const std::unique_ptr<InputMapping>& mMapping : cDefMapping) {
      mMapping->save(node);
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

  void AnalogueInput::InputMapping::save(DOMNodeWriter* node) const {
    if (cName != "") {
      DOMNodeWriter mInputNode = node->addBranch(TAG_INPUT);
      mInputNode.addAttribute("id", cName);
      cPhysicalInput->save(&mInputNode);
    }
  }

  void AnalogueInput::InputMapping::loadCustomMapping(DOMNode& node) {
    cPhysicalInput->loadCustomMapping(node);
  }

  void AnalogueInput::InputMapping::registerAssets(IAssetRegistry* assets) {
    LocalAssetRegistry mLocalRegistry(assets, cName);
    cPhysicalInput->registerAssets(&mLocalRegistry);
  }
  
  void AnalogueInput::InputMapping::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    cPhysicalInput->unregisterAssets(assets, releaser);
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
}

