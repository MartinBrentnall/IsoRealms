/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef HUD_CLUE_H
#define HUD_CLUE_H

#include <iostream>
#include <GL/glew.h>

#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/HUDComponents/IHUDGameComponent.h>

class HUDClue:public IHUDGameComponent {
  private:
  I3DModel* cClueModel;
  ICamera* cCamera;

  public:
  HUDClue(ICamera*);
    
  void setModel(I3DModel*);
  void setCamera(ICamera*);  
  
  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(unsigned int);
  float getAspectRatio();
};

#endif
