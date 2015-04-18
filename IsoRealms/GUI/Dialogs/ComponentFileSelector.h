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
#ifndef COMPONENT_FILE_SELECTOR_H
#define COMPONENT_FILE_SELECTOR_H

#include "../../System.h"
#include "../RectangularComponent.h"

#include "IFileSelectionListener.h"

class ComponentFileSelector:public RectangularComponent {
  private:
  std::vector<std::string> cDirectory;
  IFileSelectionListener* cListener;
  
  void refresh();
  
  public:
  ComponentFileSelector(IResourceAccessor*, const std::string&, IFileSelectionListener*);
  
  std::string getSelectedFile();
  void assertSelection(const std::string&, const std::string&);
};

#endif
