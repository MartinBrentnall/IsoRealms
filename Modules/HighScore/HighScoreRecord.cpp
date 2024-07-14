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
#include "HighScoreRecord.h"

#include "HighScoreTable.h"

namespace IsoRealms::HighScore {
  HighScoreRecord::HighScoreRecord(JSONObject object, HighScoreTable* parentTable) : cValues(parentTable->getFieldCount()) {
    cParentTable = parentTable;
    for (JSONObject mFieldObject : object.getArray(JSON_VALUES)) {
      std::string mFieldName   = mFieldObject.getString(JSON_FIELD);
      std::string mFieldValue  = mFieldObject.getString(JSON_VALUE);
      unsigned int mFieldIndex = cParentTable->getFieldIndex(mFieldName);
      cValues[mFieldIndex] = mFieldValue;
    }
  }

  HighScoreRecord::HighScoreRecord(std::map<std::string, std::string> record, HighScoreTable* parentTable) : cValues(parentTable->getFieldCount()) {
    cParentTable = parentTable;
    
    for (std::pair<std::string, std::string> mField : record) {
      unsigned int mFieldIndex = cParentTable->getFieldIndex(mField.first);
      cValues[mFieldIndex] = mField.second;
    }
  }

  void HighScoreRecord::save(JSONObject object) {
    JSONArray mFieldArray = object.addArray(JSON_VALUES);
    for (unsigned int i = 0; i < cValues.size(); i++) {
      JSONObject mFieldObject = mFieldArray.addObject();
      mFieldObject.addString(JSON_FIELD, cParentTable->getFieldName(i));
      mFieldObject.addString(JSON_VALUE, cValues[i]);
    }
  }

  bool HighScoreRecord::beats(const std::string& value) {
    unsigned int mComparisonFieldIndex = cParentTable->getComparisonFieldIndex();
    int mMyValue = std::stoi(cValues[mComparisonFieldIndex]);
    int mChallengerValue = std::stoi(value);
    return mMyValue >= mChallengerValue;
  }

  bool HighScoreRecord::beats(HighScoreRecord* record) {
    unsigned int mComparisonFieldIndex = cParentTable->getComparisonFieldIndex();
    int mMyValue = std::stoi(cValues[mComparisonFieldIndex]);
    int mChallengerValue = std::stoi(record->cValues[mComparisonFieldIndex]);
    return mMyValue >= mChallengerValue;
  }

  const std::string HighScoreRecord::JSON_FIELD  = "field";
  const std::string HighScoreRecord::JSON_VALUE  = "value";
  const std::string HighScoreRecord::JSON_VALUES = "values";
}
