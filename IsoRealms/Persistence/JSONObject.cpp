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
#include "JSONThing.h"

namespace IsoRealms {
  namespace {
    rapidjson::Value copiedMemberName(const std::string& name, rapidjson::Document::AllocatorType& allocator) {
      rapidjson::Value mName;
      mName.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
      return mName;
    }
  }

  JSONObject::JSONObject(JSONDocument& parent, rapidjson::Value& object) :
            cParent(parent),
            cObject(object) {
  }

  JSONArray JSONObject::addArray(const std::string& name) {
    rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
    rapidjson::Value mArray(rapidjson::kArrayType);
    cObject.AddMember(copiedMemberName(name, mAllocator), mArray, mAllocator);
    return JSONArray(cParent, cObject[name]);
  }

  JSONObject JSONObject::addObject(const std::string& name) {
    rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
    rapidjson::Value mObject(rapidjson::kObjectType);
    cObject.AddMember(copiedMemberName(name, mAllocator), mObject, mAllocator);
    return JSONObject(cParent, cObject[name]);
  }

  void JSONObject::addNull(const std::string& name) {
    rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
    rapidjson::Value mNull;
    cObject.AddMember(copiedMemberName(name, mAllocator), mNull, mAllocator);
  }

  void JSONObject::addString(const std::string& name, const std::string& value, const std::string& defaultValue) {
    if (value != defaultValue) {
      rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
      rapidjson::Value mString;
      mString.SetString(value.c_str(), value.length(), mAllocator);
      cObject.AddMember(copiedMemberName(name, mAllocator), mString, mAllocator);
    }
  }

  void JSONObject::addInteger(const std::string& name, int value, int defaultValue) {
    if (value != defaultValue) {
      rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
      rapidjson::Value mInteger;
      mInteger = value;
      cObject.AddMember(copiedMemberName(name, mAllocator), mInteger, mAllocator);
    }
  }

  void JSONObject::addFloat(const std::string& name, float value, float defaultValue) {
    if (value != defaultValue) {
      rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
      rapidjson::Value mFloat;
      mFloat = value;
      cObject.AddMember(copiedMemberName(name, mAllocator), mFloat, mAllocator);
    }
  }

  void JSONObject::addBoolean(const std::string& name, bool value, bool defaultValue) {
    if (value != defaultValue) {
      rapidjson::Document::AllocatorType& mAllocator = cParent.getDocument().GetAllocator();
      rapidjson::Value mBoolean;
      mBoolean = value;
      cObject.AddMember(copiedMemberName(name, mAllocator), mBoolean, mAllocator);
    }
  }

  bool JSONObject::hasMember(const std::string& name) const {
    return cObject.HasMember(name);
  }

  JSONArray JSONObject::getArray(const std::string& name) {
    return JSONArray(cParent, cObject[name]);
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

  JSONObject::Iterator JSONObject::begin() {
    return Iterator(*this, cObject.MemberBegin());
  }

  JSONObject::Iterator JSONObject::end() {
    return Iterator(*this, cObject.MemberEnd());
  }

  JSONObject::Iterator::Iterator(JSONObject& parent, rapidjson::Value::MemberIterator member) :
            cParent(parent),
            cMember(member) {
  }

  JSONObject::Iterator& JSONObject::Iterator::operator++() {
    ++cMember;
    return *this;
  }

  bool JSONObject::Iterator::operator!=(const JSONObject::Iterator& node) {
    return &cParent != &node.cParent || cMember != node.cMember;
  }

  JSONThing JSONObject::Iterator::operator*() {
    return JSONThing(cParent.cParent, cMember);
  }
}
