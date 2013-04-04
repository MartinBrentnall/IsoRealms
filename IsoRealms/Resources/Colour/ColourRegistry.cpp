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

