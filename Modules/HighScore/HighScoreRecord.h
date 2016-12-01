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
#ifndef HIGH_SCORE_RECORD_H
#define HIGH_SCORE_RECORD_H

#include <map>
#include <string>
#include <vector>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>

#include "IHighScoreTable.h"

class HighScoreRecord {
  private:
  IHighScoreTable* cParentTable;
  std::vector<std::string> cValues;
  
  public:
  HighScoreRecord(DOMNodeWrapper*, IHighScoreTable*);
  HighScoreRecord(std::map<std::string, std::string>, IHighScoreTable*);
  
  void save(DOMNodeWriter*);
  bool beats(const std::string&);
  bool beats(HighScoreRecord*);
};

#endif
