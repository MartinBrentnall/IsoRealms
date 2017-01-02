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
#ifndef CHECK_BOX_WITH_LABEL_H
#define CHECK_BOX_WITH_LABEL_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IValueListener.h>

#include "LookAndFeel.h"
#include "ISizedComponent.h"
#include "IValueComponent.h"

class CheckBoxWithLabel:public ISizedComponent,
                        public IValueComponent<bool>{
  private:
  bool cValue;
  std::string cLabel;
  std::vector<IValueListener<bool>*> cListeners;
  bool cHasFocus;
  
  void fireChange();
  
  public:
  CheckBoxWithLabel(const std::string&);

  /************************************\
   * Implements IValueComponent<bool> *
  \************************************/
  void setValue(bool);
  bool getValue();
  void addValueListener(IValueListener<bool>*);

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
};

#endif
