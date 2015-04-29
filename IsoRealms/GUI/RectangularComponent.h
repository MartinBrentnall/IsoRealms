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
#ifndef RECTANGULAR_COMPONENT_H
#define RECTANGULAR_COMPONENT_H

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "AbstractRectangularComponent.h"
#include "Button.h"
#include "CheckBox.h"
#include "ComponentEdgeLayout.h"
#include "Container.h"
#include "EdgeRelation.h"
#include "FlexibleGridLayoutComponent.h"
#include "GridLayoutComponent.h"
#include "IComponentHolder.h"
#include "../IComponentCustomType.h"
#include "IntegerField.h" 
#include "ISizedComponent.h"
#include "ISliderRenderer.h"
#include "IValueComponent.h"
#include "ListBox.h"
#include "MenuPopup.h"
#include "PanelComponent.h"
#include "ScrollableContainer.h"
#include "SelectableComponent.h"
#include "SliderComponent.h"
#include "TabbedContainer.h"
#include "TextEditorComponent.h"
#include "TextFieldComponent.h"
#include "TextLabelComponent.h"
#include "WrappingGridComponent.h"

class TabbedContainer;

class RectangularComponent:public ISizedComponent,
                           public IMenuContainer,
                           public IComponentHolder,
                           public IListBoxListener {
  private:
  RectangularComponent* cTopLevelComponent;
  std::vector<IRectangularComponent*> cChildren;
  std::map<std::string, ISizedComponent*> cSizedComponents;
  std::map<std::string, Button*> cCommandableComponents;
  std::map<std::string, ListBox*> cListBoxComponents;
  // TODO: Should use interface to support other components; e.g. ListBoxes
  
  // Value components.
  std::map<std::string, IValueComponent<bool>*>        cValueComponentsBoolean;
  std::map<std::string, IValueComponent<int>*>         cValueComponentsInteger;
  std::map<std::string, IValueComponent<float>*>       cValueComponentsFloat;
  std::map<std::string, IValueComponent<std::string>*> cValueComponentsString;
  std::map<std::string, IValueComponent<IColour*>*>    cValueComponentsResourceColour;
  std::map<std::string, IValueComponent<ITexture*>*>   cValueComponentsResourceTexture;
  
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
  
  void addBooleanValueComponent(const std::string&, IValueComponent<bool>*);
  void addIntegerValueComponent(const std::string&, IValueComponent<int>*);
  void addStringValueComponent(const std::string&,  IValueComponent<std::string>*);
    
  public:
  RectangularComponent();
  RectangularComponent(const std::string&, IResourceAccessor*);
  RectangularComponent(DOMNodeWrapper*, IResourceAccessor*);

  void loadComponent(DOMNodeWrapper*, IResourceAccessor*);
  void loadDialog(DOMNodeWrapper*, IRectangle*, float, IResourceAccessor*, RectangularComponent* = NULL);
  
  void setRenderer(ISliderRenderer*, const std::string&);
  void setRenderer(IPanelRenderer*, const std::string&);
  void setSelectable(const std::string&, ISelector*);
  void addFloatListener(  IValueListener<float>*,       const std::string&);
  void addStringListener( IValueListener<std::string>*, const std::string&);
  void addBooleanListener(IValueListener<bool>*,        const std::string&);
  void addIntegerListener(IValueListener<int>*,         const std::string&);
  void setBooleanValue(const std::string&, bool);
  void setIntegerValue(const std::string&, int);
  void setFloatValue(const std::string&, float);
  void setStringValue(const std::string&, const std::string&);
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
