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
#include "DigitalToAnalogueMapping.h"

namespace IsoRealms::Basics {
  DigitalToAnalogueMapping::DigitalToAnalogueMapping(IProject* project, DOMNode& node) :
            cDefInput(project, nullptr, node, nullptr, nullptr),
            cDefOutputValue(node.getFloatAttribute(ATTRIBUTE_TO_VALUE)) {
  }

  float DigitalToAnalogueMapping::getState(const sf::Event& event) const {
    for (unsigned int i = 0; i < cDefInput.getMappingCount(); i++) {
      std::shared_ptr<IDigitalInputMapping> mInput = cDefInput.getMapping(i);
      if (mInput->matches(event)) {
        if (mInput->getState(event)) {
          return cDefOutputValue;
        }
      }
    }
    return 0.0f;
  }
  
  bool DigitalToAnalogueMapping::matches(const sf::Event& event) const {
    for (unsigned int i = 0; i < cDefInput.getMappingCount(); i++) {
      std::shared_ptr<IDigitalInputMapping> mInput = cDefInput.getMapping(i);
      if (mInput->matches(event)) {
        return true;
      }
    }
    return false;
  }
  
  void DigitalToAnalogueMapping::save(DOMNodeWriter* node, const std::string& name) const {
    DOMNodeWriter mDigitalToAnalogueNode = node->addBranch("DigitalToAnalogue");
    mDigitalToAnalogueNode.addAttribute("name", name);
    mDigitalToAnalogueNode.addAttribute(ATTRIBUTE_TO_VALUE, cDefOutputValue);
    cDefInput.save(&mDigitalToAnalogueNode, nullptr);
  }
  
  std::string DigitalToAnalogueMapping::getShortName() const {
    return "TODO";
  }
  
  std::string DigitalToAnalogueMapping::getLongName() const {
    return "TODO";
  }
  
  void DigitalToAnalogueMapping::loadCustomMapping(DOMNode& node) {
    cDefInput.loadCustomMapping(node);
  }

  void DigitalToAnalogueMapping::registerAssets(IAssetRegistry* assets) {
    cDefInput.registerAssets(assets);
  }
  
  void DigitalToAnalogueMapping::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    cDefInput.unregisterAssets(assets, releaser);
  }

  const std::string DigitalToAnalogueMapping::ATTRIBUTE_TO_VALUE = "toValue";
}
