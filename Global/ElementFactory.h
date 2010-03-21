/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include "IElementFactory.h"

template<class T = IElementSet> class ElementFactory:public IElementFactory {
  private:
  T* cElementSet;

  public:
  ElementFactory(T* elementSet) {
    cElementSet = elementSet;
  }

  /******************************\
   * Implements IElementFactory *
  \******************************/
  T* getElementSet() {
    return cElementSet;
  }
};

#endif
