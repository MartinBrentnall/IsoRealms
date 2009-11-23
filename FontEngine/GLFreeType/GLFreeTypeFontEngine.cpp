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
#include "GLFreeTypeFontEngine.h"

GLFreeTypeFontEngine::GLFreeTypeFontEngine(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Font") {
      cRegisteredFonts[mNode->getAttribute("name")] = parseFont(mNode);
    }
  }
}

Font* GLFreeTypeFontEngine::parseFont(DOMNodeWrapper* node) {
  std::string mFontLocation;
  std::string mFontName;
  int mDetail = 64;
  // TODO: Prevent duplicate details / unspecified details
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "File") {
      std::string mFontFileName = mNode->getStringValue();
      mFontLocation = System::getResource(mFontFileName);
    } else if (mValueAsString == "Detail") {
      mDetail = mNode->getIntegerValue();
    }
  }
  return new Font(mFontLocation.c_str(), mDetail);
}

IFont* GLFreeTypeFontEngine::getFont(std::string name) {
  // TODO: Throw exception if it don't exist
  return cRegisteredFonts[name];
}

extern "C" IFontEngine* create(DOMNodeWrapper* node) {
  return new GLFreeTypeFontEngine(node);
}

extern "C" void destroy(IFontEngine* fontEngine) {
  delete fontEngine;
}
