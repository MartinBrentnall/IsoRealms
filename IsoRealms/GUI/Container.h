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
#ifndef CONTAINER_H
#define CONTAINER_H

#include "IComponentHolder.h"
#include "ISizedComponent.h"

class Container:public ISizedComponent,
                public IComponentHolder {
  private:
  ISizedComponent* cComponent;

  class ContainerBounds:public IComponentBoundsCalculator {
    private:
    Container* cParent;
    
    public:
    ContainerBounds(Container*);
    
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };
  
  public:
  Container();
    
  void render();
  void update(unsigned int);
  bool input(SDL_Event&);

  float getWidth();
  float getHeight();
  
  void addComponent(const std::string&, ISizedComponent*);
  void removeComponent(ISizedComponent*);
};

#endif
