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
#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "IsoRealms/Exception/ApplicationException.h"

#include "IOptions.h"

namespace IsoRealms {
  
  /**
   * A set of fixed options.
   */
  class Options : public IOptions {
    public:
    Options();
    Options(int argc, char** argv);
    Options(std::map<std::string, std::string> options);
    void addOption(const std::string& key, const std::string& value);
    bool operator==(const Options& options) const;
    std::map<std::string, std::string> getAllOptions() const;

    /***********************\
     * Implements IOptions *
    \***********************/
    std::string getOption(const std::string& key) const override;
    Options getFixedOptions() override;
    
    private:
    std::map<std::string, std::string> cOptions; /// Map of options
  };
}
