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
