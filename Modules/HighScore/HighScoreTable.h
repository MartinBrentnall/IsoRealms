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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "IsoRealms/Exception/ArgumentException.h"
#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

#include "HighScoreRecord.h"

namespace IsoRealms::HighScore {
  class HighScoreTable {
    public:
    HighScoreTable(DOMNode& node);
    HighScoreTable(std::vector<std::string> fields, const std::string& comparisonField, unsigned int maximumRecords);
    
    bool qualifies(const std::string& value);
    void insertRecord(std::map<std::string, std::string> record);
    void save(DOMNodeWriter* node);
    unsigned int getFieldCount();
    std::string getFieldName(unsigned int index);
    unsigned int getFieldIndex(const std::string& field);
    unsigned int getComparisonFieldIndex();
    
    private:
    unsigned int cMaximumRecords;
    std::string cComparisonField;
    std::vector<std::string> cHighScoreFields;
    std::vector<std::unique_ptr<HighScoreRecord>> cHighScoreRecords;
    
    void insertRecord(std::unique_ptr<HighScoreRecord> record);
  };
}
