/*
 * Copyright 2015 Martin Brentnall
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
#include "ColourRegistry.h"

IColour* ColourRegistry::getSpecialResource(const std::string& value) {
  if (value[0] == '#') {
    std::string mColour = value.substr(1);
    std::vector<std::string> mSections = Utils::splitWords(mColour, ' ');
    if (mSections.size() >= 3) {
      float mRed = atof(mSections[0].c_str());
      float mGreen = atof(mSections[1].c_str());
      float mBlue = atof(mSections[2].c_str());
      float mAlpha = 0.0f;
      if (mSections.size() == 4) {
        mAlpha = atof(mSections[3].c_str());
      }
      Colour* mColour = new Colour(mRed, mGreen, mBlue, mAlpha);
      cLiteralColours.push_back(mColour);
      return mColour;
    } else {
      std::cout << "Unexpected literal colour format" << std::endl;
      exit(1);
    }
  }
  return NULL;
}

std::string ColourRegistry::getSpecialLocation(IColour* colour) {
  for (unsigned int i = 0; i < cLiteralColours.size(); i++) {
    if (cLiteralColours[i] == colour) {
      return "#" + Utils::toString(colour->getRed()) + " " + Utils::toString(colour->getGreen()) + " " + Utils::toString(colour->getBlue()) + " " + Utils::toString(colour->getAlpha());
    }
  }
  return "";
}

