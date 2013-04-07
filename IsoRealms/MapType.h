#ifndef MAP_TYPE_H
#define MAP_TYPE_H

#include "IPlugin.h"
#include "LuaSupport/ArgumentSourceCustom.h"
#include "Map.h"
#include "Resources/ElementType/IElementType.h"

class MapType:public IElementType {
  private:
  std::map<std::string, Map*> cNamedInstances;
  IProject* cProject;
  bool cEditing;
  IResources* cResources;
  
  public:
  MapType(bool, IProject*, IResources*); 
    
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IPlugin* getElementSet();
  void loadInstance(DOMNodeWrapper*, IRuntimeContext*);
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void configureElement();
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  IElementHandler* getElementHandler();
};

#endif
