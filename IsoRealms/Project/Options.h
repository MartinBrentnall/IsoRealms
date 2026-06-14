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

#include <cstring>
#include <iostream>
#include <map>
#include <string>

namespace IsoRealms {
  
  /**
   * A set of fixed options.
   */
  class Options {
    public:
    static const Options EMPTY;

    /** Hint option: value is persisted inline in the current load object (see ComponentLoader). */
    inline static const std::string PROPERTY_INLINE = "inline";

    /** Hint option: load reference and properties immediately instead of deferring to project init. */
    inline static const std::string PROPERTY_IMMEDIATE = "immediate";

    /** Hint option: property is not loaded or saved via the property system. */
    inline static const std::string PROPERTY_NO_PERSIST = "noPersist";

    /** Hint option: property is not shown or editable in the editor. */
    inline static const std::string PROPERTY_NO_EDIT = "noEdit";

    /** Hint option: JSON member name under which nested struct properties are loaded and saved. */
    inline static const std::string PROPERTY_SCOPED = "scoped";
    
    Options();
    Options(int argc, char** argv);
    Options(const Options&) = delete;
    Options(Options&&) = delete;
    Options& operator=(const Options&) = delete;
    Options& operator=(Options&&) = delete;
    void addOption(const std::string& key, const std::string& value);
    void assign(const Options& options);
    void clear();
    bool operator==(const Options& options) const;
    std::string getOption(const std::string& key) const;
    
    private:
    std::map<std::string, std::string> cOptions; /// Map of options
  };
}
