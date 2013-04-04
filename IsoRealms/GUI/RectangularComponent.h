#ifndef RECTANGULAR_COMPONENT_H
#define RECTANGULAR_COMPONENT_H

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "AbstractRectangularComponent.h"
#include "Button.h"
#include "ComponentEdgeLayout.h"
#include "Container.h"
#include "EdgeRelation.h"
#include "FlexibleGridLayoutComponent.h"
#include "GridLayoutComponent.h"
#include "IComponentHolder.h"
#include "../IComponentCustomType.h"
#include "IFloatListener.h"
#include "../IStringListener.h"
#include "ISizedComponent.h"
#include "ISliderRenderer.h"
#include "ListBox.h"
#include "MenuPopup.h"
#include "PanelComponent.h"
#include "ScrollableContainer.h"
#include "SelectableComponent.h"
#include "SliderComponent.h"
#include "TabbedContainer.h"
#include "TextFieldComponent.h"
#include "TextLabelComponent.h"
#include "WrappingGridComponent.h"

class TabbedContainer;

class RectangularComponent:public ISizedComponent,
                           public IMenuContainer,
			   public IComponentHolder,
			   public IListBoxListener {
  private:
  std::vector<IRectangularComponent*> cChildren;
  std::map<std::string, ISizedComponent*> cSizedComponents;
  std::map<std::string, Button*> cCommandableComponents;
  std::map<std::string, ListBox*> cListBoxComponents;
  // TODO: Should use interface to support other components; e.g. ListBoxes
  std::map<std::string, TextFieldComponent*> cStringValueComponents;
  std::map<std::string, SliderComponent*> cFloatValueComponents;
  std::map<std::string, SliderComponent*> cSliders;
  std::map<std::string, IComponentHolder*> cComponentContainers;
  std::map<std::string, WrappingGridComponent*> cPaddedComponents;
  std::map<std::string, PanelComponent*> cPanels;
  std::map<std::string, SelectableComponent*> cSelectableComponents;
  std::map<std::string, TabbedContainer*> cTabbedContainers;
  std::map<IRectangularComponent*, MenuPopup*> cPopupMenus;
  IRectangularComponent* cFocusedComponent;
  MenuPopup* cActivePopupMenu;

  ListBox* getListBox(const std::string&);
  std::string getStringValue(const std::string&);
  void setComponentText(DOMNodeWrapper*, ITextComponent*);
  IComponentBoundsCalculator* getBoundsCalculator(DOMNodeWrapper*, IRectangle*, float, ISizedComponent*);
  ISizedComponent* loadSizedComponent(DOMNodeWrapper*);
  void loadFlexibleGridCells(DOMNodeWrapper*, FlexibleGridLayoutComponent*);
  void loadEvenGridCells(DOMNodeWrapper*, GridLayoutComponent*, IResourceAccessor*);
  void loadTabbedContainer(DOMNodeWrapper*, TabbedContainer*, IResourceAccessor*);
  void loadPopupMenu(IRectangularComponent*, DOMNodeWrapper*);
  void testFocusChange(SDL_Event& event);
  bool mouseButtonDown(SDL_Event&);
    
  public:
  RectangularComponent();
  RectangularComponent(const std::string&, IResourceAccessor*);
  RectangularComponent(DOMNodeWrapper*, IResourceAccessor*);

  void loadComponent(DOMNodeWrapper*, IResourceAccessor*);
  void loadDialog(DOMNodeWrapper*, IRectangle*, float, IResourceAccessor*);
  
  void setRenderer(ISliderRenderer*, const std::string&);
  void setRenderer(IPanelRenderer*, const std::string&);
  void setSelectable(const std::string&, ISelector*);
  void addFloatListener(IFloatListener*, const std::string&);
  void addStringListener(IStringListener*, const std::string&);
  void setFloatValue(const std::string&, float);
  void setStringValue(const std::string&, float);
  
  void clearListBox(const std::string&);
  void addListBoxItem(const std::string&, const std::string&);
  void addComponent(IRectangularComponent*);
  void addComponent(const std::string&, ISizedComponent*);
  void addComponentAction(const std::string&, const std::string&, ICommand*);
  void removeComponent(ISizedComponent*);
  void setFocusedComponent(IRectangularComponent*);
  void setComponentAction(const std::string&, ICommand*);
  void setComponentPadding(const std::string&, float);

  std::string getSelectedTab(const std::string&);
  std::string getValue(const std::string&);
  void setSelectedTab(const std::string&, const std::string&);
  
  virtual void renderContent() {};
  virtual void updateContent(int) {};
  virtual bool inputContent(SDL_Event&) {return false;};
  
  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void render();
  void update(unsigned int);
  bool input(SDL_Event&);
  
  float getWidth();
  float getHeight();
  
  /*****************************\
   * Implements IMenuContainer *
  \*****************************/
  void closeMenu(IMenu*);
  
  /*******************************\
   * Implements IListBoxListener *
  \*******************************/
  void assertSelection(ListBox*, const std::string&);
  
  virtual void assertSelection(const std::string&, const std::string&) {
  }
};

#endif
