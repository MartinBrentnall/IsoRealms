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
#ifndef NAMED_RESOURCE_ICON_H
#define NAMED_RESOURCE_ICON_H

#include <IsoRealms/GUI/LookAndFeel.h>

#include "Icon.h"

template <class T> class NamedResourceIcon:public Icon<T> {
  private:
  IResourceLocator* cResources;
  
  public:
  NamedResourceIcon(IResourceBrowser<T>* browser, T* resource) : Icon<T>(browser, resource) {
  }
    
  void renderIcon() {
    float mLeft = this->getLeft();
    float mBottom = this->getBottom();
    IFont* mFont = LookAndFeel::getDefaultFont();
    float mFontSize = LookAndFeel::getDefaultFontSize();
    glColor3f(1.0f, 1.0f, 1.0f);
    mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, 0, this->getResourceLocation().c_str());
  }

  float getWidth() {
    return 0.66f;
  }

  float getHeight() {
    return 0.05f;
  }
};

#endif
