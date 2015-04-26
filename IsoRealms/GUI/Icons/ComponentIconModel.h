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
#ifndef COMPONENT_ICON_MODEL_H
#define COMPONENT_ICON_MODEL_H

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/ISizedComponent.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>

class ComponentIconModel : public ISizedComponent {
  private:
  I3DModelType* cModelType;
  
  public:
  ComponentIconModel(I3DModelType*);
      
  /******************************\
   * Implements ISizedComponent *
  \******************************/
  float getWidth();
  float getHeight();
  void render();
  void update(unsigned int);
  bool input(SDL_Event&);
};

#endif
