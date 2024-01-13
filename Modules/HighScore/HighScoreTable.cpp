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

namespace IsoRealms::HighScore {
  HighScoreTable::HighScoreTable(DOMNode& node) {
    cMaximumRecords = node.getIntegerAttribute("maximumRecords");
    
    // Read fields
    for (DOMNode& mNode : node) {
      std::string mValue = mNode.getName();
      if (mValue == "Field") {
        std::string mFieldName = mNode.getAttribute("name");
        bool mCompare = mNode.getBooleanAttribute("compare");
        cHighScoreFields.push_back(mFieldName);
        if (mCompare) {
          if (cComparisonField != "") {
            std::cout << "WARNING: Comparison field already set to \"" << cComparisonField << "\".  Cannot set to \"" << mFieldName << "\"" << std::endl;
          } else {
            cComparisonField = mFieldName;
          }
        }
      } else if (mValue == "Record") {
        // Nothing to do at this stage.
      } else {
        throw ParseException("Unknown tag for HighScore/HighScoreTable: " + mValue);
      }
    }
    
    // Read records
    for (DOMNode& mNode : node) {
      std::string mValue = mNode.getName();
      if (mValue == "Record") {
        insertRecord(std::make_unique<HighScoreRecord>(mNode, this));
      } else if (mValue == "Field") {
        // Nothing to do at this stage.
      } else {
        throw ParseException("Unknown tag for HighScore/HighScoreTable: " + mValue);
      }
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
    insertRecord(std::make_unique<HighScoreRecord>(record, this));
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

  void HighScoreTable::save(DOMNodeWriter* node) {
    node->addAttribute("maximumRecords", cMaximumRecords);
    for (std::string mField : cHighScoreFields) {
      DOMNodeWriter mFieldBranch = node->addBranch("Field");
      mFieldBranch.addAttribute("name", mField);
      if (mField == cComparisonField) {
        mFieldBranch.addAttribute("compare", "true");
      }
    }
    
    for (std::unique_ptr<HighScoreRecord>& mRecord : cHighScoreRecords) {
      DOMNodeWriter mRecordBranch = node->addBranch("Record");
      mRecord->save(&mRecordBranch);
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
}
