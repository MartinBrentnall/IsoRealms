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
#include "JSONArray.h"

#include "JSONDocument.h"
#include "JSONObject.h"
#include "JSONValue.h"

namespace IsoRealms {
  JSONArray::JSONArray(JSONDocument& parent, rapidjson::Value& array) :
            cParent(parent),
            cArray(array) {
  }

  JSONObject JSONArray::addObject() {
    rapidjson::Value mObject(rapidjson::kObjectType);
    cArray.PushBack(mObject, cParent.getDocument().GetAllocator());
    return JSONObject(cParent, cArray[cArray.Size() - 1]);
  }

  JSONArray::Iterator JSONArray::begin() {
    return Iterator(*this, 0);
  }

  JSONArray::Iterator JSONArray::end() {
    return Iterator(*this, cArray.Size());
  }

  JSONArray::Iterator::Iterator(JSONArray& parent, unsigned int index) :
            cParent(parent),
            cIndex(index) {
  }

  JSONArray::Iterator& JSONArray::Iterator::operator++() {
    cIndex++;
    return *this;
  }

  bool JSONArray::Iterator::operator!=(const JSONArray::Iterator& node) {
    return &cParent != &node.cParent || cIndex != node.cIndex;
  }

  JSONValue JSONArray::Iterator::operator*() {
    return JSONValue(cParent.cParent, cParent.cArray[cIndex]);
  }
}
