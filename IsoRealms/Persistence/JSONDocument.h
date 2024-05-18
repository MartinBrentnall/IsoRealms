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
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "IsoRealms/System.h"

#include "JSONArray.h"
#include "JSONObject.h"
#include "ParseException.h"

namespace IsoRealms {
  class JSONDocument {
    public:
    JSONDocument();
    JSONDocument(const std::string& filename, bool user);

    JSONObject addObject(const std::string& name);
    JSONObject getObject(const std::string& name);
    void save(const std::string& filename);
    rapidjson::Document& getDocument();

    private:
    rapidjson::Document cDocument;
  };
}

