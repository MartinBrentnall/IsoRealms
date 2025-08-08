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

#include <map>
#include <string>
#include <vector>

#include "IsoRealms.h"

namespace IsoRealms::Tables {
  class Tables;
  
  class Table {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    Table(IProject& project, Tables& tables, IResourceData& data);
    Table(IProject& project, Tables& tables, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(PropertyMaker& owner, const Metadata& metadata);
    
    /***********************\
     * Scripting Interface *
    \***********************/
//    int getRecordCount() const;
//    Record getRecord(int index) const;
//    insertRecordAt(Record& record, int index);
    
    private:
    static const std::string JSON_FIELD_NAME;
    static const std::string JSON_FIELD_TYPE;
    static const std::string JSON_FIELDS;
    static const std::string JSON_RECORDS;

    static const std::string FIELD_TYPE_INTEGER;
    static const std::string FIELD_TYPE_STRING;

    enum class FieldType {
      STRING,
      INTEGER
    };
    
    struct Field {
      Field(JSONObject object);
      
      std::string cName;
      FieldType cType;
    };

    class Record;
    
    class Value {
      public:
      Value(FieldType type, JSONValue value);

      int getInteger() const;
      std::string getString() const;

      private:
      std::variant<int, std::string> cValue;
    };

    class Record {
      public:
      Record(Table& parent, JSONArray records);
      
      private:
      std::vector<Value> cValues;
    };
    
    std::vector<Field> cDefFields;
    std::vector<Record> cDefRecords;
    std::vector<Record> cRuntimeRecords;
    
    LuaBinding<Table> cLuaBinding;
  };
}
