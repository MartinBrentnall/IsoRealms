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
#ifndef HIGH_SCORE_TABLE_H
#define HIGH_SCORE_TABLE_H

#include <map>
#include <string>
#include <vector>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>

#include "HighScoreRecord.h"

class HighScoreTable:public IHighScoreTable {
  private:
  unsigned int cMaximumRecords;
  std::string cComparisonField;
  std::vector<std::string> cHighScoreFields;
  std::vector<HighScoreRecord*> cHighScoreRecords;
  
  void insertRecord(HighScoreRecord*);
  
  public:
  HighScoreTable(DOMNodeWrapper*);
  HighScoreTable(std::vector<std::string>, const std::string&, unsigned int);
  
  bool qualifies(const std::string&);
  void insertRecord(std::map<std::string, std::string>);
  void save(DOMNodeWriter*);
    
  /******************************\
   * Implements IHighScoreTable *
  \******************************/
  unsigned int getFieldCount();
  std::string getFieldName(unsigned int);
  unsigned int getFieldIndex(const std::string&);
  unsigned int getComparisonFieldIndex();
};

#endif
