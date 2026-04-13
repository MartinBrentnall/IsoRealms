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
#include "BooleanTrigger.h"

namespace IsoRealms::Basics {
  const std::string BooleanTrigger::JSON_ON_BECOMING_FALSE = "onBecomingFalse";
  const std::string BooleanTrigger::JSON_ON_BECOMING_TRUE  = "onBecomingTrue";
  const std::string BooleanTrigger::JSON_VALUE             = "value";

  const std::string BooleanTrigger::PROPERTY_VALUE        = "Monitored Value";
  const std::string BooleanTrigger::PROPERTY_TRUE_ACTION  = "Action on True";
  const std::string BooleanTrigger::PROPERTY_FALSE_ACTION = "Action on False";
  
  BooleanTrigger::BooleanTrigger(Basics& basics, IResourceData& data) :
            cDefValue(data, false, [this](bool value) {
              (value ? cDefTrueAction : cDefFalseAction).execute();
            }),
            cDefTrueAction(data.getDummyActionClient()),
            cDefFalseAction(data.getDummyActionClient()) {
  }
  
  BooleanTrigger::BooleanTrigger(Basics& basics, IResourceData& data, JSONObject object) :
            BooleanTrigger(basics, data) {
    cDefValue.init(object, JSON_VALUE);
    cDefTrueAction.init(object, JSON_ON_BECOMING_TRUE);
    cDefFalseAction.init(object, JSON_ON_BECOMING_FALSE);
  }

  void BooleanTrigger::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }
  
  void BooleanTrigger::save(JSONObject object) const {
    cDefValue.save(object, JSON_VALUE);
    cDefTrueAction.save(object, JSON_ON_BECOMING_TRUE);
    cDefFalseAction.save(object, JSON_ON_BECOMING_FALSE);
  }

  void BooleanTrigger::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool BooleanTrigger::renderIcon() const {
    return false;
  }

  void BooleanTrigger::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector(metadata.getPropertyData("Value"),   cDefValue);
    owner.createPropertyTreeSelector( metadata.getPropertyData("OnTrue"),  cDefTrueAction);
    owner.createPropertyTreeSelector( metadata.getPropertyData("OnFalse"), cDefFalseAction);
  }

  void BooleanTrigger::removed() {
    // Nothing to do.
  }
}
