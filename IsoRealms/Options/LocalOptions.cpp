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
#include "LocalOptions.h"

#include "Options.h"

namespace IsoRealms {
  LocalOptions::LocalOptions(const std::string& branch, IOptions& options) :
            cOptions(options),
            cBranch(branch) {
  }

  std::string LocalOptions::getOption(const std::string& key) const {
    return cOptions.getOption(cBranch + "/" + key);
  }

  Options LocalOptions::getFixedOptions() {
    Options mOptions = cOptions.getFixedOptions();
    std::map<std::string, std::string> mAllOptions = mOptions.getAllOptions();
    Options mFixedOptions;
    for (std::pair<std::string, std::string> mOption : mAllOptions) {
      if (mOption.first.rfind(cBranch + "/", 0) == 0) {
        mFixedOptions.addOption(mOption.first.substr(cBranch.size() + 1), mOption.second);
      }
    }
    return mFixedOptions;
  }
}
