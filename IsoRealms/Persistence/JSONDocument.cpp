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
#include "JSONDocument.h"

namespace IsoRealms {
  JSONDocument::JSONDocument() :
            cDocument(rapidjson::kObjectType) {
  }

  JSONDocument::JSONDocument(const std::string& filename, bool user) {
    std::ifstream mInputStream = System::openInputStream(filename, user);
    rapidjson::BasicIStreamWrapper mInputStreamWrapper(mInputStream);
    cDocument.ParseStream(mInputStreamWrapper);
  }

  JSONObject JSONDocument::addObject(const std::string& name) {
    rapidjson::Value mObject(rapidjson::kObjectType);
    cDocument.AddMember(rapidjson::StringRef(name), mObject, cDocument.GetAllocator());
    return JSONObject(*this, cDocument[name]);
  }

  JSONObject JSONDocument::getObject(const std::string& name) {
    return JSONObject(*this, cDocument[name]);
  }

  void JSONDocument::save(const std::string& filename) {
    rapidjson::StringBuffer mBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> mWriter(mBuffer);
    cDocument.Accept(mWriter);
    std::ofstream cOutputStream = System::openOutputStream(filename);
    cOutputStream << mBuffer.GetString();
    cOutputStream.close();
  }

  rapidjson::Document& JSONDocument::getDocument() {
    return cDocument;
  }
}
