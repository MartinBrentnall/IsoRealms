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
#include "Options.h"

#include "IsoRealms/Exception/ApplicationException.h"

namespace IsoRealms {
  Options::Options() {
    // Nothing to do.
  }

  Options::Options(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-t") == 0) {
        if (i >= argc + 1) {
          throw ApplicationException("Project type argument missing");
        }
        cOptions["type"] = argv[i + 1];
        i++;
      } else if (strcmp(argv[i], "-o") == 0) {
        if (i >= argc + 2) {
          throw ApplicationException("Project option argument missing");
        }
        cOptions[argv[i + 1]] = argv[i + 2];
        i += 2;
      } else {
        cOptions["file"] = argv[i];
      }
    }
  }
  
  Options::Options(std::map<std::string, std::string> options) :
            cOptions(options) {
  }

  void Options::addOption(const std::string& key, const std::string& value) {
    std::map<std::string, std::string>::const_iterator mOption = cOptions.find(key);
    if (mOption != cOptions.end()) {
      std::cout << "WARNING: Options::addOption: Option key \"" << key << "\" already specified" << std::endl;
    }
    cOptions[key] = value;
  }

  bool Options::operator==(const Options& options) const {
    if (options.cOptions.size() != cOptions.size()) {
      return false;
    }
    for (std::pair<std::string, std::string> mOption : cOptions) {
      std::string mOptionValue = options.getOption(mOption.first);
      if (mOptionValue != mOption.second) {
        return false;
      }
    }
    return true;
  }

  std::map<std::string, std::string> Options::getAllOptions() const {
    return cOptions;
  }

  std::string Options::getOption(const std::string& option) const {
    std::map<std::string, std::string>::const_iterator mOption = cOptions.find(option);
    if (mOption != cOptions.end()) {
      return mOption->second;
    }
    return "";
  }
  
  Options Options::getFixedOptions() {
    return *this;
  }
}
