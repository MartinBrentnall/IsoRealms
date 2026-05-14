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
#include "JSONDocument.h"

#include "IsoRealms/Assets/Client/File.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Utils.h"

#include "JSONArray.h"
#include "JSONObject.h"
#include "ParseException.h"

namespace IsoRealms {
  JSONDocument::JSONDocument() :
            cDocument(rapidjson::kObjectType) {
  }

  JSONDocument::JSONDocument(const std::string& filename, bool user) {
    std::ifstream mInputStream = System::openInputStream(filename, user);
    rapidjson::BasicIStreamWrapper mInputStreamWrapper(mInputStream);
    cDocument.ParseStream(mInputStreamWrapper);
    if (cDocument.HasParseError()) {
      throw ParseException("JSONDocument::JSONDocument: Error parsing document \"" + filename + "\" (" + (user ? "User File" : "Program File") + "), error code " + Utils::toString(static_cast<int>(cDocument.GetParseError())));
    }
  }

  JSONDocument::JSONDocument(File& file) :
            JSONDocument(file.getRelativePath(), file.isUser()) {
  }

  JSONObject JSONDocument::addObject(const std::string& name) {
    rapidjson::Value mObject(rapidjson::kObjectType);
    cDocument.AddMember(rapidjson::StringRef(name), mObject, cDocument.GetAllocator());
    return JSONObject(*this, cDocument[name]);
  }

  JSONObject JSONDocument::getObject(const std::string& name) {
    return JSONObject(*this, cDocument[name]);
  }

  std::string JSONDocument::getString(const std::string& name, const std::string& defaultValue) const {
    return cDocument.HasMember(name) ? cDocument[name].GetString() : defaultValue;
  }

  bool JSONDocument::hasMember(const std::string& name) const {
    return cDocument.HasMember(name);
  }

  void JSONDocument::save(const std::string& filename) {
    rapidjson::StringBuffer mBuffer;
//    rapidjson::Writer<rapidjson::StringBuffer> mWriter(mBuffer);
    rapidjson::PrettyWriter<rapidjson::StringBuffer> mWriter(mBuffer);
    mWriter.SetIndent(' ', 2);
    cDocument.Accept(mWriter);
    std::ofstream cOutputStream = System::openOutputStream(filename);
    cOutputStream << mBuffer.GetString();
    cOutputStream.close();
  }

  rapidjson::Document& JSONDocument::getDocument() {
    return cDocument;
  }
}
