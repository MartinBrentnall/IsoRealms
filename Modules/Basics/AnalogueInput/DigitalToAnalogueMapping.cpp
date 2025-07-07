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
#include "DigitalToAnalogueMapping.h"

namespace IsoRealms::Basics {
  const std::string DigitalToAnalogueMapping::JSON_DIGITAL_TO_ANALOGUE = "digitalToAnalogue";
  const std::string DigitalToAnalogueMapping::JSON_NAME                = "name";
  const std::string DigitalToAnalogueMapping::JSON_TO_VALUE            = "toValue";
  const std::string DigitalToAnalogueMapping::JSON_TYPE                = "type";

  const std::string DigitalToAnalogueMapping::TYPE_DIGITAL_TO_ANALOGUE = "DigitalToAnalogue";

  DigitalToAnalogueMapping::DigitalToAnalogueMapping(IProject& project, Basics& basics, JSONObject object) :
            cDefInput(project, basics, object),
            cDefOutputValue(object.getFloat(JSON_TO_VALUE)) {
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
  
  void DigitalToAnalogueMapping::save(JSONObject object, const std::string& name) const {
    object.addString(JSON_TYPE, TYPE_DIGITAL_TO_ANALOGUE);
    object.addString(JSON_NAME, name);
    object.addFloat(JSON_TO_VALUE, cDefOutputValue);
    cDefInput.save(object);
  }

  std::string DigitalToAnalogueMapping::getShortName() const {
    return "TODO";
  }
  
  std::string DigitalToAnalogueMapping::getLongName() const {
    return "TODO";
  }
  
  void DigitalToAnalogueMapping::loadCustomMapping(JSONObject object) {
    cDefInput.loadCustomMapping(object);
  }

  void DigitalToAnalogueMapping::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    cDefInput.registerAssets(assets, parentID);
  }
}
