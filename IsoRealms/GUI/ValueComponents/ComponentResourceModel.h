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
#ifndef COMPONENT_RESOURCE_MODEL_H
#define COMPONENT_RESOURCE_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IValueListener.h>
#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/GUI/ISizedComponent.h>
#include <IsoRealms/GUI/IValueComponent.h>

class ComponentResourceModel : public ISizedComponent,
                               public IValueComponent<I3DModelType*>,
                               public IResourceSelectionListener<I3DModelType> {
  private:
  I3DModelType* cValue;
  std::vector<IValueListener<I3DModelType*>*> cListeners;
  bool cHasFocus;
  float cSize;
  IResourceSelector* cResourceSelector;
  
  void fireChange();
  
  public:
  ComponentResourceModel(float, IResourceSelector*);

  /************************************\
   * Implements IValueComponent<bool> *
  \************************************/
  void setValue(I3DModelType*);
  I3DModelType* getValue();
  void addValueListener(IValueListener<I3DModelType*>*);

  /***************************************\
   * Implements IComponentSizeCalculator *
  \***************************************/
  float getWidth();
  float getHeight();  

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  virtual void gainedFocus();
  virtual void lostFocus();  

  virtual void resourceSelected(I3DModelType*);
};

#endif
