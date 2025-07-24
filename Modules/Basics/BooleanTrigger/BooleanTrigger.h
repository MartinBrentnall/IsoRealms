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
#pragma once

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition monitors a Boolean asset for changes and executes
   * actions when the monitored value changes to true/false.
   */
  class BooleanTrigger final {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    BooleanTrigger(IProject& project, Basics& basics, IResourceData& data);
    BooleanTrigger(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner);
    
    private:

    // JSON members.
    static const std::string JSON_ON_BECOMING_FALSE;
    static const std::string JSON_ON_BECOMING_TRUE;
    static const std::string JSON_VALUE;

    // Property names.
    static const std::string PROPERTY_VALUE;
    static const std::string PROPERTY_TRUE_ACTION;
    static const std::string PROPERTY_FALSE_ACTION;

    // Definition data.
    Boolean cDefValue;      /// Boolean value to monitor.
    Action cDefTrueAction;  /// Action to perform when value changes to true.
    Action cDefFalseAction; /// Action to perform when value changes to false.
  };
}
