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

#include <iostream>

#define RAPIDJSON_HAS_STDSTRING 1

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace IsoRealms {
  class JSONDocument;
  class JSONObject;

  class JSONArray {
    public:
    class Iterator {
      private:
      JSONArray& cParent;
      unsigned int cIndex;

      public:
      Iterator(JSONArray& object, unsigned int index);

      Iterator& operator++();
      bool operator!=(const Iterator&);
      JSONObject operator*();
    };

    JSONArray(JSONDocument& parent, rapidjson::Value& object);
    JSONObject addObject();
    Iterator begin();
    Iterator end();

    private:
    JSONDocument& cParent;
    rapidjson::Value& cArray;
  };
}


