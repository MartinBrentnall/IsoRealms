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
  const std::string BooleanTrigger::TAG_ACTION_ON_TRUE  = "ActionOnTrue";
  const std::string BooleanTrigger::TAG_ACTION_ON_FALSE = "ActionOnFalse";
  const std::string BooleanTrigger::TAG_VALUE           = "Value";

  const std::string BooleanTrigger::PROPERTY_VALUE        = "Monitored Value";
  const std::string BooleanTrigger::PROPERTY_TRUE_ACTION  = "Action on True";
  const std::string BooleanTrigger::PROPERTY_FALSE_ACTION = "Action on False";
  
  BooleanTrigger::BooleanTrigger(IProject* project, Basics* basics) :
            cDefValue(project, false, [this](bool value) {(value ? cDefTrueAction : cDefFalseAction).execute();}),
            cDefTrueAction(project),
            cDefFalseAction(project) {
  }
  
  BooleanTrigger::BooleanTrigger(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            BooleanTrigger(project, basics) {
    cDefValue.init(node.getNode(TAG_VALUE));
    cDefTrueAction.init(node, TAG_ACTION_ON_TRUE);
    cDefFalseAction.init(node, TAG_ACTION_ON_FALSE);
  }

  void BooleanTrigger::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void BooleanTrigger::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // Nothing to do.
  }
  
  void BooleanTrigger::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefValue.save(node, TAG_VALUE);
    cDefTrueAction.save(node, TAG_ACTION_ON_TRUE);
    cDefFalseAction.save(node, TAG_ACTION_ON_FALSE);
  }

  void BooleanTrigger::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool BooleanTrigger::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> BooleanTrigger::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }
}
