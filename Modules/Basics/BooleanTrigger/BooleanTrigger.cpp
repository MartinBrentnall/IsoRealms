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
#include "BooleanTrigger.h"

namespace IsoRealms::Basics {
  const std::string BooleanTrigger::JSON_ON_BECOMING_FALSE = "onBecomingFalse";
  const std::string BooleanTrigger::JSON_ON_BECOMING_TRUE  = "onBecomingTrue";
  const std::string BooleanTrigger::JSON_VALUE             = "value";

  const std::string BooleanTrigger::PROPERTY_VALUE        = "Monitored Value";
  const std::string BooleanTrigger::PROPERTY_TRUE_ACTION  = "Action on True";
  const std::string BooleanTrigger::PROPERTY_FALSE_ACTION = "Action on False";
  
  BooleanTrigger::BooleanTrigger(IProject& project, Basics& basics, IResourceData& data) :
            cDefValue(data, false, [this](bool value) {
              (value ? cDefTrueAction : cDefFalseAction).execute();
            }),
            cDefTrueAction(data),
            cDefFalseAction(data) {
  }
  
  BooleanTrigger::BooleanTrigger(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            BooleanTrigger(project, basics, data) {
    cDefValue.init(object, JSON_VALUE);
    cDefTrueAction.init(object, JSON_ON_BECOMING_TRUE);
    cDefFalseAction.init(object, JSON_ON_BECOMING_FALSE);
  }

  void BooleanTrigger::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }
  
  void BooleanTrigger::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    // Nothing to do.
  }
  
  void BooleanTrigger::save(JSONObject object, IAssetIdentifier& identifier) const {
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

  std::vector<std::unique_ptr<IProperty>> BooleanTrigger::getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Boolean>>("Monitored Value", "Boolean to monitor for value changes.", cDefValue));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action on True",   "Action to execute when the monitored boolean changes value from FALSE to TRUE", cDefTrueAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action on False",  "Action to execute when the monitored boolean changes value from TRUE to FALSE", cDefFalseAction));
    return mProperties;
  }
}
