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

#include <chrono>
#include <format>

#include "IsoRealms.h"

namespace IsoRealms::UI {
  class StringTime final : public IString {
    public:

    // Constructors.
    StringTime(const Metadata& metadata, IComponentData& owner);
    StringTime(const Metadata& metadata, IComponentData& owner, JSONObject object);

    /**********************\
     * Implements IString *
    \**********************/


    void getResourceProperties(IComponentDefiner& definer) override;

    bool isDefaultConfiguration() const override;
    std::string getValue() const override;

    /*********************************\
     * Implements IResource via IString *
    \*********************************/
    std::string getConversionPath() const override;
    bool isConfigurable() const override;

    private:
    class FormatElement {
      
      public:
      // true if this element is a token (like {m}, {s:02}, etc), false if literal text
      bool isToken;

      // valid if isToken == false
      std::string literal;

      // valid if isToken == true
      std::string unit;   // e.g. "m", "s", "ms"
      int width = 0;      // 0 means no width specified
      bool zeroPad = false;

      FormatElement(const std::string& lit) :
                isToken(false),
                literal(lit),
                unit(),
                width(0),
                zeroPad(false) {
      }

      FormatElement(const std::string& u, int w, bool zp) :
                isToken(true),
                literal(),
                unit(u),
                width(w),
                zeroPad(zp) {
      }
    };

    // JSON members.
    inline static const std::string JSON_FORMAT = "format";
    inline static const std::string JSON_VALUE  = "value";

    inline static const std::string DEFAULT_FORMAT = "{m}:{s:02}.{ms:2}";

    // Definition data.
    Integer cDefValue;                   /// Value in milliseconds to convert to string format.
    std::string format = DEFAULT_FORMAT; /// Format of the time.

    // Runtime data.
    std::vector<FormatElement> cFormatElements;

    void parseFormatString();
  };
}

