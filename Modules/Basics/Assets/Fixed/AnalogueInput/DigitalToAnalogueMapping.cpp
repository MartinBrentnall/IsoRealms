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
            cMetadata(metadata),
            cDefName(metadata.getPropertyData("DefaultName").getName()),
            cDefControl(owner),
            cDefOutputValue(0.0f) {
  }

  DigitalToAnalogueMapping::DigitalToAnalogueMapping(const Metadata& metadata, IComponentData& owner, JSONObject object) :
            cMetadata(metadata),
            cDefName(object.getString(JSON_NAME)),
            cDefControl(owner, object),
            cDefOutputValue(object.getFloat(JSON_TO_VALUE)) {
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
  
  void DigitalToAnalogueMapping::loadCustomMapping(JSONObject object) {
    cDefControl.loadCustomMapping(object);
  }

  void DigitalToAnalogueMapping::registerAssets(ComponentAssetRegistry& assets) {
    cDefControl.registerAssets(assets, cDefName);
  }

  bool DigitalToAnalogueMapping::renderAssetIcon() const {
    return false;
  }

  void DigitalToAnalogueMapping::saveAsset(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    object.addFloat(JSON_TO_VALUE, cDefOutputValue);
//    cDefControl.save(object);
  }

  void DigitalToAnalogueMapping::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeString(JSON_NAME,     [this]() {return cDefName;},        [this](const std::string& name) {cDefName        = name;});
    owner.createPropertyNativeFloat( JSON_TO_VALUE, [this]() {return cDefOutputValue;}, [this](float toValue)           {cDefOutputValue = toValue;});
    cDefControl.getProperties(owner, cMetadata);
  }
  
  bool DigitalToAnalogueMapping::isDefaultConfiguration() const {
    return true;
  }
}
