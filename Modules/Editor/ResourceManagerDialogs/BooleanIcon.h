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
#ifndef BOOLEAN_ICON_H
#define BOOLEAN_ICON_H

#include <IsoRealms/GUI/CheckBox.h>
#include <IsoRealms/GUI/Icon.h>
#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/GUI/FixedSizeLayout.h>

class BooleanIcon:public Icon<IBoolean>,
                  public IComponentLocation {
  public:
  CheckBox cCheckBox;
  BooleanIcon(IResourceBrowser<IBoolean>*, IBoolean*);
    
  void renderIcon();
  float getWidth();
  float getHeight();
  
  float getX();
  float getY();
};

#endif

