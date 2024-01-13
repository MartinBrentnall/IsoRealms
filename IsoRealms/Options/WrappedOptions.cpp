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
#include "WrappedOptions.h"

#include "Options.h"

namespace IsoRealms {
  WrappedOptions::WrappedOptions(IOptions* options) :
            cWrappedOptions(options) {
  }
    
  void WrappedOptions::addOption(const std::string& key, const std::string& value) {
    std::map<std::string, std::string>::const_iterator mOption = cOptions.find(key);
    if (mOption != cOptions.end()) {
      std::cout << "WARNING: WrappedOptions::addOption: Option key \"" << key << "\" already specified" << std::endl;
    }
    cOptions[key] = value;
  }
    
  std::string WrappedOptions::getOption(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator mOption = cOptions.find(key);
    if (mOption != cOptions.end()) {
      return mOption->second;
    }
    return cWrappedOptions->getOption(key);
  }

  Options WrappedOptions::getFixedOptions() {
    Options mOptions = cWrappedOptions->getFixedOptions();
    for (std::pair<std::string, std::string> mOption : cOptions) {
      mOptions.addOption(mOption.first, mOption.second);
    }
    return mOptions;
  }
}
