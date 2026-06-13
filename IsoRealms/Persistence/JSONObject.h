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

#define RAPIDJSON_HAS_STDSTRING 1

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace IsoRealms {
  class JSONDocument;
  class JSONArray;
  class JSONThing;

  class JSONObject {
    public:
    class Iterator {
      private:
      JSONObject& cParent;
      rapidjson::Value::MemberIterator cMember;

      public:
      Iterator(JSONObject& object, rapidjson::Value::MemberIterator member);

      Iterator& operator++();
      bool operator!=(const Iterator&);
      JSONThing operator*();
    };

    Iterator begin();
    Iterator end();
    JSONObject(JSONDocument& parent, rapidjson::Value& object);
    JSONArray addArray(const std::string& name);
    JSONObject addObject(const std::string& name);
    void addNull(const std::string& name);
    void addString(const std::string& name, const std::string& value, const std::string& defaultValue = "");
    void addInteger(const std::string& name, int value, int defaultValue = 0);
    void addFloat(const std::string& name, float value, float defaultValue = 0.0f);
    void addBoolean(const std::string& name, bool value, bool defaultValue = false);
    bool hasMember(const std::string& name) const;
    JSONArray getArray(const std::string& name);
    JSONArray getArray(const std::string& name) const;
    JSONObject getObject(const std::string& name) const;
    std::string getString(const std::string& name, const std::string& defaultValue = "") const;
    int getInteger(const std::string& name, int defaultValue = 0) const;
    float getFloat(const std::string& name, float defaultValue = 0.0f) const;
    bool getBoolean(const std::string& name, bool defaultValue = false) const;

    private:
    JSONDocument& cParent;
    rapidjson::Value& cObject;
  };
}
