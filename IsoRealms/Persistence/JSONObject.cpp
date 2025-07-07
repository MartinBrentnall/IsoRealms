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
#include "JSONObject.h"

#include "JSONArray.h"
#include "JSONDocument.h"

namespace IsoRealms {
  JSONObject::JSONObject(JSONDocument& parent, rapidjson::Value& object) :
            cParent(parent),
            cObject(object) {
  }

  JSONArray JSONObject::addArray(const std::string& name) {
    rapidjson::Value mArray(rapidjson::kArrayType);
    cObject.AddMember(rapidjson::StringRef(name), mArray, cParent.getDocument().GetAllocator());
    return JSONArray(cParent, cObject[name]);
  }

  JSONObject JSONObject::addObject(const std::string& name) {
    rapidjson::Value mObject(rapidjson::kObjectType);
    cObject.AddMember(rapidjson::StringRef(name), mObject, cParent.getDocument().GetAllocator());
    return JSONObject(cParent, cObject[name]);
  }

  void JSONObject::addString(const std::string& name, const std::string& value, const std::string& defaultValue) {
    if (value != defaultValue) {
      rapidjson::Value mString;
      mString.SetString(value.c_str(), value.length(), cParent.getDocument().GetAllocator());
      cObject.AddMember(rapidjson::StringRef(name), mString, cParent.getDocument().GetAllocator());
    }
  }

  void JSONObject::addInteger(const std::string& name, int value, int defaultValue) {
    if (value != defaultValue) {
      rapidjson::Value mInteger;
      mInteger = value;
      cObject.AddMember(rapidjson::StringRef(name), mInteger, cParent.getDocument().GetAllocator());
    }
  }

  void JSONObject::addFloat(const std::string& name, float value, float defaultValue) {
    if (value != defaultValue) {
      rapidjson::Value mFloat;
      mFloat = value;
      cObject.AddMember(rapidjson::StringRef(name), mFloat, cParent.getDocument().GetAllocator());
    }
  }

  void JSONObject::addBoolean(const std::string& name, bool value, bool defaultValue) {
    if (value != defaultValue) {
      rapidjson::Value mBoolean;
      mBoolean = value;
      cObject.AddMember(rapidjson::StringRef(name), mBoolean, cParent.getDocument().GetAllocator());
    }
  }

  bool JSONObject::hasMember(const std::string& name) const {
    return cObject.HasMember(name);
  }

  JSONArray JSONObject::getArray(const std::string& name) const {
    return JSONArray(cParent, cObject[name]);
  }

  JSONObject JSONObject::getObject(const std::string& name) const {
    return JSONObject(cParent, cObject[name]);
  }

  std::string JSONObject::getString(const std::string& name, const std::string& defaultValue) const {
    return cObject.HasMember(name) ? cObject[name].GetString() : defaultValue;
  }

  bool JSONObject::getBoolean(const std::string& name, bool defaultValue) const {
    return cObject.HasMember(name) ? cObject[name].GetBool() : defaultValue;
  }

  int JSONObject::getInteger(const std::string& name, int defaultValue) const {
    return cObject.HasMember(name) ? cObject[name].GetInt() : defaultValue;
  }

  float JSONObject::getFloat(const std::string& name, float defaultValue) const {
    return cObject.HasMember(name) ? cObject[name].GetFloat() : defaultValue;
  }
}
