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
#include "Table.h"

namespace IsoRealms::Tables {
  Table::Table(Tables& tables, IComponentData& data) :
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  Table::Table(Tables& tables, IComponentData& data, JSONObject object) :
            Table(tables, data) {
    for (JSONValue mFieldValue : object.getArray("fields")) {
      cDefFields.emplace_back(Field(mFieldValue.getObject()));
    }
    for (JSONValue mRecordValue : object.getArray("records")) {
      cDefRecords.emplace_back(Record(*this, mRecordValue.getArray()));
    }
  }

  void Table::registerAssets(ComponentAssetRegistry& assets) {
    // TODO: Implement this.
  }

  void Table::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    // TODO: Implement this.
  }
  
  Table::Field::Field(JSONObject object) :
            cName(object.getString("name")),
            cType(object.getString("type") == FIELD_TYPE_INTEGER ? FieldType::INTEGER : FieldType::STRING) {
  }
  
  Table::Value::Value(FieldType type, JSONValue value) {
    switch (type) {
      case FieldType::INTEGER: {cValue = value.getInteger();                       break;}
      case FieldType::STRING:  {cValue = value.getString();                        break;}
      default:                 {std::cout << "TODO: Throw exception" << std::endl; break;}
    }
  }
  
  int Table::Value::getInteger() const {
    return std::get<int>(cValue);
  }
  
  std::string Table::Value::getString() const {
    return std::get<std::string>(cValue);
  }
  
  Table::Record::Record(Table& parent, JSONArray records) {
    for (JSONValue mRecordValue : records) {
      cValues.emplace_back(Value(parent.cDefFields[cValues.size()].cType, mRecordValue));
    }
  }
}
