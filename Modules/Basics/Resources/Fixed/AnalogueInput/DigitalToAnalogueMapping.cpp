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

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  DigitalToAnalogueMapping::DigitalToAnalogueMapping(const Metadata& metadata, IComponentData& owner) :
            cDefName(metadata.getPropertyData("DefaultName").getName()),
            cDefControl(owner) {
  }

  void DigitalToAnalogueMapping::publish(ResourcePublisher& publisher) {
    cDefControl.publish(publisher, cDefName);
  }

  std::string DigitalToAnalogueMapping::getName() const {
    return cDefName;
  }

  float DigitalToAnalogueMapping::getState(const sf::Event& event) const {
    for (unsigned int i = 0; i < cDefControl.getMappingCount(); i++) {
      std::shared_ptr<DigitalInput> mInput = cDefControl.getMapping(i);
      if ((*mInput)->matches(event)) {
        if ((*mInput)->getState(event)) {
          return cDefOutputValue;
        }
      }
    }
    return 0.0f;
  }
  
  bool DigitalToAnalogueMapping::matches(const sf::Event& event) const {
    for (unsigned int i = 0; i < cDefControl.getMappingCount(); i++) {
      std::shared_ptr<DigitalInput> mInput = cDefControl.getMapping(i);
      if ((*mInput)->matches(event)) {
        return true;
      }
    }
    return false;
  }
  
  std::string DigitalToAnalogueMapping::getShortName() const {
    return cDefName;
  }
  
  std::string DigitalToAnalogueMapping::getLongName() const {
    return cDefName + " -> " + Utils::toString(cDefOutputValue);
  }

  std::string DigitalToAnalogueMapping::getLocalizedName() const {
    return cDefName;
  }
  
  void DigitalToAnalogueMapping::defineResource(IComponentDefiner& definer) {
    definer.propertyString("name",   [this]() {return cDefName;},        [this](const std::string& name) {cDefName        = name;});
    definer.propertyFloat("toValue", [this]() {return cDefOutputValue;}, [this](float toValue)           {cDefOutputValue = toValue;});
    cDefControl.define(definer);
  }
}
