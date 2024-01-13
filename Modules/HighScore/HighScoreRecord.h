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
#include <string>
#include <vector>

#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms::HighScore {
  class HighScoreTable;
  
  class HighScoreRecord {
    public:
    HighScoreRecord(DOMNode& node, HighScoreTable* parentTable);
    HighScoreRecord(std::map<std::string, std::string> record, HighScoreTable* parentTable);
    
    void save(DOMNodeWriter* node);
    bool beats(const std::string& value);
    bool beats(HighScoreRecord* record);
    
    private:
    HighScoreTable* cParentTable;
    std::vector<std::string> cValues;
  };
}
