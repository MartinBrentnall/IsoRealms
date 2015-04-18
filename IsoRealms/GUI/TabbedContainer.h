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
#ifndef TABBED_CONTAINER_H
#define TABBED_CONTAINER_H

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/Font/IFont.h>

#include "AbstractRectangularComponent.h"
#include "LookAndFeel.h"

class TabbedContainer:public AbstractRectangularComponent {
  private:
  class TabLayout:public IComponentBoundsCalculator {
    private:
    TabbedContainer* cParent;
    
    public:
    TabLayout(TabbedContainer*);
    
    /*****************************************\
     * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };
    
  class Tab {
    public:
    std::string cActualName;
    std::string cName;
    IRectangularComponent* cComponent;
    
    Tab(const std::string&, const std::string&, IRectangularComponent*);
  };
    
  std::vector<Tab*> cTabs;
  TabLayout* cTabLayout;
  unsigned int cActiveTab;
    
  bool mouseButtonDown(SDL_Event&);

  public:
  TabbedContainer();
    
  void addTab(const std::string&, const std::string&, IRectangularComponent*);
  IComponentBoundsCalculator* getTabLayout();

  std::string getSelectedTab();
  void setSelectedTab(const std::string&);
  
  /*******************************************\
   * Implements AbstractRectangularComponent *
  \*******************************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
};

#endif
