/*
 * Copyright 2016 Martin Brentnall
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

HighScoreTable::HighScoreTable(DOMNodeWrapper* node) {
  cMaximumRecords = node->getIntegerAttribute("maximumRecords");
  
  // Read fields
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Field") {
      std::string mFieldName = mNode->getAttribute("name");
      bool mCompare = mNode->getBooleanAttribute("compare");
      cHighScoreFields.push_back(mFieldName);
      if (mCompare) {
        if (cComparisonField != "") {
          std::cout << "WARNING: Comparison field already set to \"" << cComparisonField << "\".  Cannot set to \"" << mFieldName << "\"" << std::endl;
        } else {
          cComparisonField = mFieldName;
        }
      }
    }
  }
  
  // Read records
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Record") {
      HighScoreRecord* mRecord = new HighScoreRecord(mNode, this);
      insertRecord(mRecord);
    }
  }
}

HighScoreTable::HighScoreTable(std::vector<std::string> fields, const std::string& comparisonField, unsigned int maximumRecords) {
  cHighScoreFields = fields;
  cComparisonField = comparisonField;
  cMaximumRecords = maximumRecords;
}

void HighScoreTable::insertRecord(HighScoreRecord* record) {
  for (unsigned int i = 0; i < cHighScoreRecords.size(); i++) {
    if (!cHighScoreRecords[i]->beats(record)) {
      cHighScoreRecords.insert(cHighScoreRecords.begin() + i, record);
      while (cHighScoreRecords.size() > cMaximumRecords) {
        cHighScoreRecords.pop_back();
      }
      return;
    }
  }

  if (cHighScoreRecords.size() < cMaximumRecords) {
    cHighScoreRecords.push_back(record);
  }
}

void HighScoreTable::insertRecord(std::map<std::string, std::string> record) {
  HighScoreRecord* mRecord = new HighScoreRecord(record, this);
  insertRecord(mRecord);
}

bool HighScoreTable::qualifies(const std::string& value) {
  if (cHighScoreRecords.size() < 20) {
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
    DOMNodeWriter* mFieldBranch = node->addBranch("Field");
    mFieldBranch->addAttribute("name", mField);
    if (mField == cComparisonField) {
      mFieldBranch->addAttribute("compare", "true");
    }
  }
  
  for (HighScoreRecord* mRecord : cHighScoreRecords) {
    DOMNodeWriter* mRecordBranch = node->addBranch("Record");
    mRecord->save(mRecordBranch);
  }
}

unsigned int HighScoreTable::getFieldCount() {
  return cHighScoreFields.size();
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
  std::cout << "WARNING: Field \"" << field << "\" not found in high score table" << std::endl;
  exit(1);
  return 0;
}

unsigned int HighScoreTable::getComparisonFieldIndex() {
  return getFieldIndex(cComparisonField);
}
