/*
 * Copyright 2009 Martin Brentnall
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
#include "Utils.h"

std::string Utils::toString(int number) {
  std::stringstream mStringStream;
  mStringStream << number;
  std::string mString = mStringStream.str();
  return mString;
}

std::string Utils::toString(unsigned int number) {
  std::stringstream mStringStream;
  mStringStream << number;
  std::string mString = mStringStream.str();
  return mString;
}

std::string Utils::toString(float number) {
  std::stringstream mStringStream;
  mStringStream << number;
  std::string mString = mStringStream.str();
  return mString;
}

std::vector<std::string> Utils::split(const std::string& string, char splitChar) {
  std::string mString = string;
  std::vector<std::string> mSplitStrings;
  std::string::size_type mSplitPos = mString.find_first_of(splitChar);
  while (mSplitPos != std::string::npos) {
    std::string mSplitString = mString.substr(0, mSplitPos);
    mSplitStrings.push_back(mSplitString);
    mString = mString.substr(mSplitPos + 1);
    mSplitPos = mString.find_first_of(splitChar);
  }
  mSplitStrings.push_back(mString);
  return mSplitStrings;
}

// TODO: Move this into a generic "split into words" function class... or something
std::vector<std::string> Utils::splitWords(const std::string& words, char splitChar) {
  std::string mWords = words;
  std::vector<std::string> mSplitWords;
  std::string::size_type mWordStart = mWords.find_first_not_of(splitChar);
  while (mWordStart != std::string::npos) {
    mWords = mWords.substr(mWordStart);
    std::string::size_type mWordEnd = mWords.find_first_of(splitChar);
    if (mWordEnd != std::string::npos) {
      std::string mAlignWord = mWords.substr(0, mWordEnd);
      mWords = mWords.substr(mWordEnd);
      mSplitWords.push_back(mAlignWord);
      mWordStart = mWords.find_first_not_of(splitChar);
    } else {
      mSplitWords.push_back(mWords);
      mWordStart = std::string::npos;
    }
  }
  return mSplitWords;
}

