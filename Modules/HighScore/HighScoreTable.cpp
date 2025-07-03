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
#include "HighScoreTable.h"

#include "IsoRealms/Persistence.h"

namespace IsoRealms::HighScore {
  HighScoreTable::HighScoreTable(JSONObject object) {
    cMaximumRecords = object.getInteger(JSON_RECORD_LIMIT);
    
    // Read fields
    for (JSONObject mFieldObject : object.getArray(JSON_FIELDS)) {
      std::string mFieldName = mFieldObject.getString(JSON_NAME);
      bool mCompare = mFieldObject.getBoolean(JSON_COMPARE);
      cHighScoreFields.push_back(mFieldName);
      if (mCompare) {
        if (cComparisonField != "") {
          std::cout << "WARNING: Comparison field already set to \"" << cComparisonField << "\".  Cannot set to \"" << mFieldName << "\"" << std::endl;
        } else {
          cComparisonField = mFieldName;
        }
      }
    }
    
    // Read records
    for (JSONObject mRecordObject : object.getArray(JSON_RECORDS)) {
      insertRecord(std::make_unique<HighScoreRecord>(mRecordObject, *this));
    }
  }

  HighScoreTable::HighScoreTable(std::vector<std::string> fields, const std::string& comparisonField, unsigned int maximumRecords) {
    cHighScoreFields = fields;
    cComparisonField = comparisonField;
    cMaximumRecords = maximumRecords;
  }

  void HighScoreTable::insertRecord(std::unique_ptr<HighScoreRecord> record) {
    for (unsigned int i = 0; i < cHighScoreRecords.size(); i++) {
      if (!cHighScoreRecords[i]->beats(record.get())) {
        cHighScoreRecords.insert(cHighScoreRecords.begin() + i, std::move(record));
        while (cHighScoreRecords.size() > cMaximumRecords) {
          cHighScoreRecords.pop_back();
        }
        return;
      }
    }

    if (cHighScoreRecords.size() < cMaximumRecords) {
      cHighScoreRecords.emplace_back(std::move(record));
    }
  }

  void HighScoreTable::insertRecord(std::map<std::string, std::string> record) {
    insertRecord(std::make_unique<HighScoreRecord>(record, *this));
  }

  bool HighScoreTable::qualifies(const std::string& value) {
    if (cHighScoreRecords.size() < cMaximumRecords) {
      return true;
    }
    if (cHighScoreRecords[cHighScoreRecords.size() - 1]->beats(value)) {
      return false;
    }
    return true;
  }

  void HighScoreTable::save(JSONObject object) {
    object.addInteger(JSON_RECORD_LIMIT, cMaximumRecords);
    JSONArray mFieldArray = object.addArray(JSON_FIELDS);
    for (std::string mField : cHighScoreFields) {
      JSONObject mFieldObject = mFieldArray.addObject();
      mFieldObject.addString(JSON_NAME, mField);
      mFieldObject.addBoolean(JSON_COMPARE, mField == cComparisonField);
    }
    JSONArray mRecordArray = object.addArray(JSON_RECORDS);
    for (std::unique_ptr<HighScoreRecord>& mRecord : cHighScoreRecords) {
      JSONObject mRecordObject = mRecordArray.addObject();
      mRecord->save(mRecordObject);
    }
  }

  unsigned int HighScoreTable::getFieldCount() {
    return static_cast<unsigned int>(cHighScoreFields.size());
  }

  std::string HighScoreTable::getFieldName(unsigned int index) {
    return cHighScoreFields[index];
  }

  unsigned int HighScoreTable::getFieldIndex(const std::string& field) {
    for (unsigned int i = 0; i < cHighScoreFields.size(); i++) {
      if (cHighScoreFields[i] == field) {
        return i;
      }
    }
    throw ArgumentException("ERROR: HighScoreTable::getFieldIndex: Field \"" + field + "\" not found.");
  }

  unsigned int HighScoreTable::getComparisonFieldIndex() {
    return getFieldIndex(cComparisonField);
  }

  const std::string HighScoreTable::JSON_COMPARE      = "compare";
  const std::string HighScoreTable::JSON_FIELDS       = "fields";
  const std::string HighScoreTable::JSON_NAME         = "name";
  const std::string HighScoreTable::JSON_RECORD_LIMIT = "recordLimit";
  const std::string HighScoreTable::JSON_RECORDS      = "records";
}
