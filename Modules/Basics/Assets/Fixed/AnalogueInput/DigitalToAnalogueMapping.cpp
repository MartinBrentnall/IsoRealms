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
  DigitalToAnalogueMapping::DigitalToAnalogueMapping(const Metadata& metadata, IResourceData& owner) :
            cMetadata(metadata),
            cDefName("TODO Name this mapping"),
            cDefInput(owner),
            cDefOutputValue(0.0f) {
  }

  DigitalToAnalogueMapping::DigitalToAnalogueMapping(const Metadata& metadata, IResourceData& owner, JSONObject object) :
            cMetadata(metadata),
            cDefName(object.getString(JSON_NAME)),
            cDefInput(owner, object),
            cDefOutputValue(object.getFloat(JSON_TO_VALUE)) {
  }

  std::string DigitalToAnalogueMapping::getName() const {
    return cDefName;
  }

  float DigitalToAnalogueMapping::getState(const sf::Event& event) const {
    for (unsigned int i = 0; i < cDefInput.getMappingCount(); i++) {
      std::shared_ptr<DigitalInputMapping> mInput = cDefInput.getMapping(i);
      if ((*mInput)->matches(event)) {
        if ((*mInput)->getState(event)) {
          return cDefOutputValue;
        }
      }
    }
    return 0.0f;
  }
  
  bool DigitalToAnalogueMapping::matches(const sf::Event& event) const {
    for (unsigned int i = 0; i < cDefInput.getMappingCount(); i++) {
      std::shared_ptr<DigitalInputMapping> mInput = cDefInput.getMapping(i);
      if ((*mInput)->matches(event)) {
        return true;
      }
    }
    return false;
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

  void DigitalToAnalogueMapping::registerAssets(ResourceAssetRegistry& assets) {
    cDefInput.registerAssets(assets, cDefName);
  }

  bool DigitalToAnalogueMapping::renderAssetIcon() const {
    return false;
  }

  void DigitalToAnalogueMapping::saveAsset(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    object.addFloat(JSON_TO_VALUE, cDefOutputValue);
    cDefInput.save(object);
  }

  void DigitalToAnalogueMapping::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeString(cMetadata.getPropertyData("Name"),    [this]() {return cDefName;},        [this](const std::string& name) {cDefName        = name;});
    //owner.createPropertyTreeSelector(cMetadata.getPropertyData("Input"),   &cDefInput);
    owner.createPropertyNativeFloat( cMetadata.getPropertyData("ToValue"), [this]() {return cDefOutputValue;}, [this](float toValue)           {cDefOutputValue = toValue;});
  }
  
  bool DigitalToAnalogueMapping::isDefaultConfiguration() const {
    return true;
  }
}
